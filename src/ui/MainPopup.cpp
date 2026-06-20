#include "MainPopup.hpp"
#include "../generator/LevelBuilder.hpp"
#include "../ai/GroqClient.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <chrono>

using namespace geode::prelude;

static constexpr float PW = 400.f;
static constexpr float PH = 340.f;

static const ccColor3B COL_ACTIVE  = {255, 215,  0};
static const ccColor3B COL_IDLE    = {160, 160, 160};

// =============================================================================
//  create
// =============================================================================
MainPopup* MainPopup::create(LevelEditorLayer* editor) {
    auto* p = new MainPopup();
    if (p && p->initAnchored(PW, PH, editor)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// =============================================================================
//  setup
// =============================================================================
bool MainPopup::setup(LevelEditorLayer* editor) {
    m_editor = editor;
    m_config = GeneratorConfig{};

    m_diffBtns  = CCArray::create(); m_diffBtns->retain();
    m_lenBtns   = CCArray::create(); m_lenBtns->retain();
    m_biomeBtns = CCArray::create(); m_biomeBtns->retain();
    m_modeBtns  = CCArray::create(); m_modeBtns->retain();
    m_songBtns  = CCArray::create(); m_songBtns->retain();

    auto* root = m_mainLayer;

    auto* title = CCLabelBMFont::create("AI Level Generator", "goldFont.fnt");
    title->setScale(0.70f);
    title->setPosition({PW * 0.5f, PH - 18.f});
    root->addChild(title);

    buildTabBar(root);

    for (int i = 0; i < 4; ++i) {
        m_tabs[i] = CCNode::create();
        m_tabs[i]->setPosition({0.f, 0.f});
        m_tabs[i]->setVisible(false);
        root->addChild(m_tabs[i]);
    }

    buildDiffTab();
    buildLengthStyleTab();
    buildSongTab();
    buildAITab();
    showTab(0);

    // ── Generate button ───────────────────────────────────────────────────
    auto* genMenu = CCMenu::create();
    genMenu->setPosition({0.f, 0.f});
    root->addChild(genMenu, 5);

    auto* genBtn = makeBtn("GENERATE LEVEL", menu_selector(MainPopup::onGenerate), 160.f, 34.f);
    genBtn->setPosition({PW * 0.5f, 28.f});
    genMenu->addChild(genBtn);

    // ── Seed row ─────────────────────────────────────────────────────────
    auto* seedLblH = CCLabelBMFont::create("Seed:", "bigFont.fnt");
    seedLblH->setScale(0.38f);
    seedLblH->setAnchorPoint({0.f, 0.5f});
    seedLblH->setPosition({14.f, 60.f});
    root->addChild(seedLblH);

    char buf[16]; snprintf(buf, sizeof(buf), "%u", m_config.seed);
    m_seedLbl = CCLabelBMFont::create(buf, "bigFont.fnt");
    m_seedLbl->setScale(0.38f);
    m_seedLbl->setAnchorPoint({0.f, 0.5f});
    m_seedLbl->setPosition({70.f, 60.f});
    root->addChild(m_seedLbl);

    auto* seedMenu = CCMenu::create();
    seedMenu->setPosition({0.f, 0.f});
    root->addChild(seedMenu, 5);

    auto* randBtn = makeBtn("Random", menu_selector(MainPopup::onRandomSeed), 58.f, 22.f);
    randBtn->setPosition({230.f, 60.f});
    seedMenu->addChild(randBtn);

    // ── Status label (shown during AI loading) ────────────────────────────
    m_statusLbl = CCLabelBMFont::create("", "bigFont.fnt");
    m_statusLbl->setScale(0.38f);
    m_statusLbl->setColor({255, 200, 50});
    m_statusLbl->setPosition({PW * 0.5f, 60.f});
    m_statusLbl->setVisible(false);
    root->addChild(m_statusLbl, 6);

    return true;
}

// =============================================================================
//  buildTabBar  (4 tabs now)
// =============================================================================
void MainPopup::buildTabBar(CCNode* root) {
    auto* tabMenu = CCMenu::create();
    tabMenu->setPosition({0.f, 0.f});
    root->addChild(tabMenu, 4);

    const char* labels[] = {"Difficulty", "Length/Style", "Song", "IA / Groq"};
    float xPos[]         = {52.f, 155.f, 258.f, 355.f};
    float widths[]       = {98.f, 98.f, 98.f, 88.f};

    for (int i = 0; i < 4; ++i) {
        auto* bg = CCScale9Sprite::create("GJ_button_02.png");
        bg->setContentSize({widths[i], 26.f});

        auto* lbl = CCLabelBMFont::create(labels[i], "bigFont.fnt");
        lbl->setScale(0.34f);
        lbl->setPosition({widths[i] * 0.5f, 13.f});
        bg->addChild(lbl);

        auto* btn = CCMenuItemSpriteExtra::create(bg, this, menu_selector(MainPopup::onTab));
        btn->setTag(i);
        btn->setPosition({xPos[i], PH - 45.f});
        tabMenu->addChild(btn);
    }
}

// =============================================================================
//  showTab
// =============================================================================
void MainPopup::showTab(int idx) {
    for (int i = 0; i < 4; ++i)
        if (m_tabs[i]) m_tabs[i]->setVisible(i == idx);
    m_activeTab = idx;
}

// =============================================================================
//  buildDiffTab  – Tab 0
// =============================================================================
void MainPopup::buildDiffTab() {
    CCNode* t = m_tabs[0];
    auto* menu = CCMenu::create();
    menu->setPosition({0.f, 0.f});
    t->addChild(menu);

    sectionLabel("DIFFICULTY", 14.f, PH - 80.f, t);

    const char* dlbls[] = {"Normal","Hard","Harder","Insane",
                            "Easy\nDemon","Med\nDemon","Hard\nDemon",
                            "Insane\nDemon","Extreme\nDemon"};
    float xs[] = {26,68,110,152,194,236,278,320,362};
    for (int i = 0; i < 9; ++i) {
        auto* bg = CCScale9Sprite::create("GJ_button_04.png");
        bg->setContentSize({38.f, 38.f});
        auto* lbl = CCLabelBMFont::create(dlbls[i], "chatFont.fnt");
        lbl->setScale(0.36f);
        lbl->setPosition({19.f, 19.f});
        bg->addChild(lbl);
        auto* btn = CCMenuItemSpriteExtra::create(bg, this, menu_selector(MainPopup::onDifficulty));
        btn->setTag(i);
        btn->setPosition({xs[i], PH - 115.f});
        menu->addChild(btn);
        m_diffBtns->addObject(btn);
    }
    highlightRow(m_diffBtns, static_cast<int>(m_config.difficulty));

    sectionLabel("GAME MODE", 14.f, PH - 160.f, t);

    const char* mlbls[] = {"Cube Only","Ship Heavy","Wave Heavy","Mixed","Ball+UFO","Insanity"};
    float mxs[]         = { 40.f, 112.f, 184.f, 256.f, 328.f, 200.f};
    float mys[]         = { PH-190.f, PH-190.f, PH-190.f, PH-190.f, PH-190.f, PH-220.f};
    for (int i = 0; i < 6; ++i) {
        auto* btn = makeBtn(mlbls[i], menu_selector(MainPopup::onMode), 68.f, 24.f);
        btn->setTag(i);
        btn->setPosition({mxs[i], mys[i]});
        menu->addChild(btn);
        m_modeBtns->addObject(btn);
    }
    highlightRow(m_modeBtns, static_cast<int>(m_config.modeProfile));
}

// =============================================================================
//  buildLengthStyleTab  – Tab 1
// =============================================================================
void MainPopup::buildLengthStyleTab() {
    CCNode* t = m_tabs[1];
    auto* menu = CCMenu::create();
    menu->setPosition({0.f, 0.f});
    t->addChild(menu);

    sectionLabel("LENGTH", 14.f, PH - 80.f, t);

    const char* llbls[] = {"Tiny","Short","Medium","Long","XL","Marathon","Nightmare"};
    float lxs[]         = {30,75,120,165,210,260,330};
    for (int i = 0; i < 7; ++i) {
        auto* btn = makeBtn(llbls[i], menu_selector(MainPopup::onLength), 58.f, 24.f);
        btn->setTag(i);
        btn->setPosition({lxs[i], PH - 110.f});
        menu->addChild(btn);
        m_lenBtns->addObject(btn);
    }
    highlightRow(m_lenBtns, static_cast<int>(m_config.length));

    sectionLabel("BIOME / VISUAL STYLE", 14.f, PH - 145.f, t);

    const char* blbls[] = {"Classic","Neon","Dark","Galaxy","Lava","Ice","Nature","Minimal"};
    float bxs[]         = {26,76,126,176,226,276,326,376};
    for (int i = 0; i < 8; ++i) {
        auto* btn = makeBtn(blbls[i], menu_selector(MainPopup::onBiome), 46.f, 22.f);
        btn->setTag(i);
        btn->setPosition({bxs[i], PH - 170.f});
        menu->addChild(btn);
        m_biomeBtns->addObject(btn);
    }
    highlightRow(m_biomeBtns, static_cast<int>(m_config.biome));

    sectionLabel("DECORATION DENSITY", 14.f, PH - 205.f, t);

    auto* minusBtn = makeBtn("-", menu_selector(MainPopup::onDecoMinus), 28.f, 24.f);
    minusBtn->setPosition({110.f, PH - 225.f});
    menu->addChild(minusBtn);

    char dbuf[8]; snprintf(dbuf, sizeof(dbuf), "%d%%", m_config.decoLevel);
    m_decoLbl = CCLabelBMFont::create(dbuf, "bigFont.fnt");
    m_decoLbl->setScale(0.45f);
    m_decoLbl->setPosition({200.f, PH - 225.f});
    t->addChild(m_decoLbl);

    auto* plusBtn = makeBtn("+", menu_selector(MainPopup::onDecoPlus), 28.f, 24.f);
    plusBtn->setPosition({290.f, PH - 225.f});
    menu->addChild(plusBtn);
}

// =============================================================================
//  buildSongTab  – Tab 2
// =============================================================================
void MainPopup::buildSongTab() {
    CCNode* t = m_tabs[2];
    auto* menu = CCMenu::create();
    menu->setPosition({0.f, 0.f});
    t->addChild(menu);

    sectionLabel("SONG PRESET", 14.f, PH - 80.f, t);

    const char* slbls[] = {
        "Electroman Adv.",
        "Geom. Dominator",
        "Theory of Ev. 2",
        "Club Step",
        "Deadlocked"
    };
    for (int i = 0; i < 5; ++i) {
        auto* btn = makeBtn(slbls[i], menu_selector(MainPopup::onSong), 140.f, 26.f);
        btn->setTag(i);
        float bx = (i % 2 == 0) ? 110.f : 290.f;
        float by = PH - 110.f - (i / 2) * 34.f;
        btn->setPosition({bx, by});
        menu->addChild(btn);
        m_songBtns->addObject(btn);
    }
    highlightRow(m_songBtns, static_cast<int>(m_config.song));

    auto* info = CCLabelBMFont::create(
        "Song plays inside the editor when you\ntest your level with the play button.",
        "chatFont.fnt");
    info->setScale(0.45f);
    info->setColor({180, 180, 180});
    info->setPosition({PW * 0.5f, PH - 230.f});
    t->addChild(info);
}

// =============================================================================
//  buildAITab  – Tab 3
// =============================================================================
void MainPopup::buildAITab() {
    CCNode* t = m_tabs[3];
    auto* menu = CCMenu::create();
    menu->setPosition({0.f, 0.f});
    t->addChild(menu);

    // ── Section: API Key ─────────────────────────────────────────────────
    sectionLabel("GROQ API KEY  (gratis en console.groq.com)", 14.f, PH - 78.f, t);

    // Load saved key
    auto savedKey = Mod::get()->getSavedValue<std::string>("groq-api-key", "");

    m_apiInput = geode::TextInput::create(340.f, "Pega aqui tu API key de Groq...");
    m_apiInput->setPosition({PW * 0.5f, PH - 110.f});
    m_apiInput->setString(savedKey);
    t->addChild(m_apiInput);

    // "Obtener key" button
    auto* keyBtn = makeBtn("Obtener key gratis", menu_selector(MainPopup::onGetKey), 155.f, 24.f);
    keyBtn->setPosition({PW * 0.5f, PH - 140.f});
    menu->addChild(keyBtn);

    // ── Section: Prompt ───────────────────────────────────────────────────
    sectionLabel("DESCRIBE TU NIVEL (opcional)", 14.f, PH - 168.f, t);

    auto savedPrompt = Mod::get()->getSavedValue<std::string>("last-prompt", "");
    m_promptInput = geode::TextInput::create(340.f, "Ej: nivel neon imposible con wave y ship...");
    m_promptInput->setPosition({PW * 0.5f, PH - 200.f});
    m_promptInput->setString(savedPrompt);
    t->addChild(m_promptInput);

    // ── Info text ─────────────────────────────────────────────────────────
    auto* info = CCLabelBMFont::create(
        "La IA configura dificultad, bioma, modos y\ndecoracion. Luego presiona GENERATE LEVEL.",
        "chatFont.fnt");
    info->setScale(0.40f);
    info->setColor({180, 180, 180});
    info->setPosition({PW * 0.5f, PH - 242.f});
    t->addChild(info);
}

// =============================================================================
//  Helpers
// =============================================================================
CCMenuItemSpriteExtra* MainPopup::makeBtn(const char* label, SEL_MenuHandler sel, float w, float h) {
    auto* bg = CCScale9Sprite::create("GJ_button_04.png");
    bg->setContentSize({w, h});
    auto* lbl = CCLabelBMFont::create(label, "bigFont.fnt");
    lbl->setScale(std::min(0.44f, (w - 8.f) / (strlen(label) * 14.f + 1.f)));
    lbl->setPosition({w * 0.5f, h * 0.5f});
    bg->addChild(lbl);
    return CCMenuItemSpriteExtra::create(bg, this, sel);
}

CCLabelBMFont* MainPopup::sectionLabel(const char* txt, float x, float y, CCNode* parent) {
    auto* l = CCLabelBMFont::create(txt, "bigFont.fnt");
    l->setScale(0.34f);
    l->setColor({220, 180, 80});
    l->setAnchorPoint({0.f, 0.5f});
    l->setPosition({x, y});
    parent->addChild(l);
    return l;
}

void MainPopup::highlightRow(CCArray* arr, int tag) {
    for (unsigned i = 0; i < arr->count(); ++i) {
        auto* btn = static_cast<CCMenuItemSpriteExtra*>(arr->objectAtIndex(i));
        btn->setColor(btn->getTag() == tag ? COL_ACTIVE : COL_IDLE);
    }
}

void MainPopup::refreshDecoLabel() {
    char buf[8]; snprintf(buf, sizeof(buf), "%d%%", m_config.decoLevel);
    m_decoLbl->setString(buf);
}

void MainPopup::refreshSeedLabel() {
    char buf[16]; snprintf(buf, sizeof(buf), "%u", m_config.seed);
    m_seedLbl->setString(buf);
}

void MainPopup::setLoading(bool on) {
    if (on) {
        m_statusLbl->setString("Preguntando a la IA...");
        m_statusLbl->setVisible(true);
        m_seedLbl->setVisible(false);
    } else {
        m_statusLbl->setVisible(false);
        m_seedLbl->setVisible(true);
    }
}

// =============================================================================
//  Callbacks
// =============================================================================
void MainPopup::onTab(CCObject* sender) {
    showTab(static_cast<CCNode*>(sender)->getTag());
}

void MainPopup::onGetKey(CCObject*) {
    geode::utils::web::openLinkInBrowser("https://console.groq.com/keys");
}

void MainPopup::onDifficulty(CCObject* sender) {
    int t = static_cast<CCNode*>(sender)->getTag();
    m_config.difficulty = static_cast<Difficulty>(t);
    highlightRow(m_diffBtns, t);
}

void MainPopup::onLength(CCObject* sender) {
    int t = static_cast<CCNode*>(sender)->getTag();
    m_config.length = static_cast<LevelLength>(t);
    highlightRow(m_lenBtns, t);
}

void MainPopup::onBiome(CCObject* sender) {
    int t = static_cast<CCNode*>(sender)->getTag();
    m_config.biome = static_cast<Biome>(t);
    highlightRow(m_biomeBtns, t);
}

void MainPopup::onMode(CCObject* sender) {
    int t = static_cast<CCNode*>(sender)->getTag();
    m_config.modeProfile = static_cast<ModeProfile>(t);
    highlightRow(m_modeBtns, t);
}

void MainPopup::onDecoPlus(CCObject*) {
    m_config.decoLevel = std::min(100, m_config.decoLevel + 10);
    refreshDecoLabel();
}

void MainPopup::onDecoMinus(CCObject*) {
    m_config.decoLevel = std::max(0, m_config.decoLevel - 10);
    refreshDecoLabel();
}

void MainPopup::onSong(CCObject* sender) {
    int t = static_cast<CCNode*>(sender)->getTag();
    m_config.song = static_cast<SongChoice>(t);
    highlightRow(m_songBtns, t);
}

void MainPopup::onRandomSeed(CCObject*) {
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    m_config.seed = static_cast<uint32_t>(now & 0xFFFFFFFFu);
    refreshSeedLabel();
}

// =============================================================================
//  onGenerate  – calls Groq AI if key present, else builds directly
// =============================================================================
void MainPopup::onGenerate(CCObject*) {
    if (!m_editor) {
        FLAlertLayer::create("Error", "No editor layer found.", "OK")->show();
        return;
    }

    std::string apiKey;
    std::string prompt;
    if (m_apiInput)  apiKey = m_apiInput->getString();
    if (m_promptInput) prompt = m_promptInput->getString();

    // Save values for next session
    if (!apiKey.empty()) Mod::get()->setSavedValue<std::string>("groq-api-key", apiKey);
    if (!prompt.empty()) Mod::get()->setSavedValue<std::string>("last-prompt", prompt);
    Mod::get()->setSavedValue<int>("last-seed", static_cast<int>(m_config.seed));

    if (!apiKey.empty()) {
        // ── AI path ──────────────────────────────────────────────────────
        setLoading(true);
        auto body = GroqClient::buildBody(prompt);

        m_webListener.bind([this](geode::utils::web::WebTask::Event* e) {
            if (auto* response = e->getValue()) {
                setLoading(false);
                if (response->ok()) {
                    auto raw = response->string().unwrapOrDefault("");
                    auto result = GroqClient::parseResponse(raw, m_config);
                    if (result.ok) {
                        log::info("[AILevelGen] AI config: diff={} len={} biome={}",
                            result.config.difficultyName(),
                            result.config.lengthName(),
                            result.config.biomeName());
                        LevelBuilder builder(result.config);
                        builder.build(m_editor);
                        FLAlertLayer::create(
                            "IA Gen",
                            "Nivel generado por IA!\n" +
                            result.description + "\n\n"
                            "Dificultad: <cy>" + result.config.difficultyName() + "</c>\n"
                            "Bioma: <cp>" + result.config.biomeName() + "</c>",
                            "Awesome!"
                        )->show();
                        this->onClose(nullptr);
                    } else {
                        FLAlertLayer::create("Error IA", result.error.c_str(), "OK")->show();
                    }
                } else {
                    auto code = response->code();
                    FLAlertLayer::create("Error IA",
                        fmt::format("HTTP {}: Verifica tu API key de Groq.", code).c_str(),
                        "OK")->show();
                }
            }
        });

        m_webListener.setFilter(
            geode::utils::web::WebRequest()
                .header("Authorization", fmt::format("Bearer {}", apiKey))
                .header("Content-Type", "application/json")
                .bodyString(body)
                .post("https://api.groq.com/openai/v1/chat/completions")
        );
    } else {
        // ── Direct path (no AI key) ───────────────────────────────────────
        log::info("[AILevelGen] Generating (no AI): diff={} len={} biome={} seed={}",
                  m_config.difficultyName(), m_config.lengthName(),
                  m_config.biomeName(), m_config.seed);

        LevelBuilder builder(m_config);
        builder.build(m_editor);

        FLAlertLayer::create(
            "AI Level Generator",
            "Nivel generado!\n"
            "Dificultad: <cy>" + m_config.difficultyName() + "</c>\n"
            "Longitud:   <cg>" + m_config.lengthName() + "</c>\n"
            "Bioma:      <cp>" + m_config.biomeName() + "</c>\n\n"
            "<cs>Tip: ve al tab IA para usar Groq gratis!</c>",
            "Awesome!"
        )->show();

        this->onClose(nullptr);
    }
}
