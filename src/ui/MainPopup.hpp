#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../GeneratorConfig.hpp"

using namespace geode::prelude;

class MainPopup : public geode::Popup<LevelEditorLayer*> {
protected:
    bool setup(LevelEditorLayer* editor) override;
public:
    static MainPopup* create(LevelEditorLayer* editor);
private:
    void buildTabBar(CCNode* root);
    void showTab(int idx);
    void buildDiffTab();
    void buildLengthStyleTab();
    void buildSongTab();
    void buildAITab();

    CCMenuItemSpriteExtra* makeBtn(const char* label, SEL_MenuHandler sel,
                                   float w = 60.f, float h = 24.f);
    CCLabelBMFont* sectionLabel(const char* txt, float x, float y, CCNode* parent);
    void highlightRow(CCArray* arr, int tag);

    void onTab(CCObject* sender);
    void onDifficulty(CCObject* sender);
    void onLength(CCObject* sender);
    void onBiome(CCObject* sender);
    void onMode(CCObject* sender);
    void onDecoPlus(CCObject*);
    void onDecoMinus(CCObject*);
    void onSong(CCObject* sender);
    void onRandomSeed(CCObject*);
    void onGenerate(CCObject*);
    void onGetKey(CCObject*);
    void setLoading(bool on);

    void refreshDecoLabel();
    void refreshSeedLabel();

    LevelEditorLayer* m_editor   = nullptr;
    GeneratorConfig   m_config;

    CCNode* m_tabs[4]  = {nullptr, nullptr, nullptr, nullptr};
    int     m_activeTab = 0;

    CCArray* m_diffBtns  = nullptr;
    CCArray* m_lenBtns   = nullptr;
    CCArray* m_biomeBtns = nullptr;
    CCArray* m_modeBtns  = nullptr;
    CCArray* m_songBtns  = nullptr;

    CCLabelBMFont* m_decoLbl    = nullptr;
    CCLabelBMFont* m_seedLbl    = nullptr;
    CCLabelBMFont* m_statusLbl  = nullptr;

    geode::TextInput* m_apiInput    = nullptr;
    geode::TextInput* m_promptInput = nullptr;

    EventListener<geode::utils::web::WebTask> m_webListener;
};
