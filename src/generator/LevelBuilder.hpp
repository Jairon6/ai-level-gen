#pragma once
#include <Geode/Geode.hpp>
#include "../GeneratorConfig.hpp"
#include "../IDs.hpp"
#include "Pattern.hpp"
#include "DifficultyEngine.hpp"
#include "PatternLibrary.hpp"
#include <random>
#include <functional>

using namespace geode::prelude;
using ProgressCallback = std::function<void(float)>;

class LevelBuilder {
public:
    explicit LevelBuilder(const GeneratorConfig& cfg);
    void build(LevelEditorLayer* editor, ProgressCallback onProgress=nullptr);
private:
    void fillGroundAndCeiling(LevelEditorLayer* editor, float endX);
    void buildSections(LevelEditorLayer* editor, ProgressCallback cb);
    void placeDecoration(LevelEditorLayer* editor, float endX);
    void placeSpeedPortals(LevelEditorLayer* editor, float endX);
    void placeTransition(PatternMode from,PatternMode to,float x,LevelEditorLayer* editor);
    void placePattern(const Pattern& pat,float anchorX,LevelEditorLayer* editor);
    void placeObj(int id,float x,float y,float rot,LevelEditorLayer* editor);
    const Pattern* pickPattern(int diffInt,PatternMode mode);
    const Pattern* pickDecoPat();
    GeneratorConfig      m_cfg;
    DifficultyEngine     m_diff;
    mutable std::mt19937 m_rng;
};
