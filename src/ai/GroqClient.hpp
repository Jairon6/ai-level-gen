#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../GeneratorConfig.hpp"
#include <string>
#include <functional>
#include <algorithm>
#include <cstring>
#include <cctype>

using namespace geode::prelude;

struct GroqResult {
    bool ok = false;
    GeneratorConfig config;
    std::string description;
    std::string error;
};

namespace GroqClient {

// ── JSON string escaping ──────────────────────────────────────────────────────
inline std::string escapeJson(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (unsigned char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += (char)c; break;
        }
    }
    return out;
}

// ── Simple JSON field extractors (no matjson needed) ─────────────────────────

// Returns the value of "key": N  where N is an integer
inline int extractInt(const std::string& json, const std::string& key, int def) {
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return def;
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == ':' || json[pos] == '\t')) ++pos;
    if (pos >= json.size()) return def;
    char* end = nullptr;
    long v = std::strtol(json.c_str() + pos, &end, 10);
    if (end == json.c_str() + pos) return def;
    return static_cast<int>(v);
}

// Returns the unescaped value of "key": "value"
inline std::string extractString(const std::string& json, const std::string& key,
                                  const std::string& def = "") {
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return def;
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == ':' || json[pos] == '\t')) ++pos;
    if (pos >= json.size() || json[pos] != '"') return def;
    ++pos;
    std::string result;
    bool esc = false;
    while (pos < json.size()) {
        char c = json[pos++];
        if (esc) { result += c; esc = false; }
        else if (c == '\\') esc = true;
        else if (c == '"') break;
        else result += c;
    }
    return result;
}

// Extracts the "content" field from a Groq API response.
// The response is: {"choices":[{"message":{"content":"..."}}],...}
inline std::string extractContent(const std::string& rawJson) {
    // Find the content field after "message"
    auto msgPos = rawJson.find("\"message\"");
    if (msgPos == std::string::npos) return "";
    auto contentPos = rawJson.find("\"content\"", msgPos);
    if (contentPos == std::string::npos) return "";
    // Now extract the string value
    auto colon = rawJson.find(':', contentPos + 9);
    if (colon == std::string::npos) return "";
    auto quote = rawJson.find('"', colon + 1);
    if (quote == std::string::npos) return "";
    ++quote;
    std::string result;
    bool esc = false;
    while (quote < rawJson.size()) {
        char c = rawJson[quote++];
        if (esc) { result += c; esc = false; }
        else if (c == '\\') esc = true;
        else if (c == '"') break;
        else result += c;
    }
    return result;
}

// ── Build Groq request body ───────────────────────────────────────────────────
inline std::string buildBody(const std::string& userPrompt) {
    const std::string sys =
        "You are a Geometry Dash extreme demon level designer AI. "
        "Return ONLY a valid JSON object (no markdown) with exactly these fields: "
        "difficulty (int 0-8, 8=ExtremeDemon), "
        "length (int 0-6, 6=Nightmare), "
        "biome (int 0-7: 0=Classic 1=Neon 2=Dark 3=Galaxy 4=Lava 5=Ice 6=Nature 7=Minimal), "
        "mode_profile (int 0-5: 0=CubeOnly 1=ShipHeavy 2=WaveHeavy 3=Mixed 4=BallUFO 5=Insanity), "
        "song (int 0-4: 0=Electroman 1=Geometrical 2=Theory 3=ClubStep 4=Deadlocked), "
        "seed (positive int), "
        "deco_level (int 0-100), "
        "description (string max 80 chars about the level style).";

    std::string prompt = userPrompt.empty()
        ? "Generate a creative and brutal extreme demon level"
        : userPrompt;

    return fmt::format(
        "{{\"model\":\"llama-3.1-8b-instant\","
        "\"response_format\":{{\"type\":\"json_object\"}},"
        "\"max_tokens\":350,"
        "\"temperature\":0.9,"
        "\"messages\":["
        "{{\"role\":\"system\",\"content\":\"{}\"}},"
        "{{\"role\":\"user\",\"content\":\"{}\"}}"
        "]}}",
        escapeJson(sys), escapeJson(prompt)
    );
}

// ── Parse Groq response ───────────────────────────────────────────────────────
inline GroqResult parseResponse(const std::string& rawJson, const GeneratorConfig& base) {
    GroqResult res;
    res.config = base;

    // API error?
    if (rawJson.find("\"error\"") != std::string::npos &&
        rawJson.find("\"choices\"") == std::string::npos) {
        res.error = extractString(rawJson, "message", "Groq API error");
        return res;
    }

    std::string content = extractContent(rawJson);
    if (content.empty()) {
        res.error = "Empty response from AI";
        return res;
    }

    // Parse the flat JSON object the AI returns
    res.config.difficulty  = static_cast<Difficulty>(
        std::clamp(extractInt(content, "difficulty",  7), 0, 8));
    res.config.length      = static_cast<LevelLength>(
        std::clamp(extractInt(content, "length",      3), 0, 6));
    res.config.biome       = static_cast<Biome>(
        std::clamp(extractInt(content, "biome",       1), 0, 7));
    res.config.modeProfile = static_cast<ModeProfile>(
        std::clamp(extractInt(content, "mode_profile",3), 0, 5));
    res.config.song        = static_cast<SongChoice>(
        std::clamp(extractInt(content, "song",        0), 0, 4));
    res.config.seed        = static_cast<uint32_t>(
        std::max(1, extractInt(content, "seed", 1337)));
    res.config.decoLevel   = std::clamp(
        extractInt(content, "deco_level", 70), 0, 100);
    res.description        = extractString(content, "description", "AI generated level");
    res.ok = true;
    return res;
}

} // namespace GroqClient
