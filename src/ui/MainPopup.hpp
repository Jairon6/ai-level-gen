#pragma once
#include <Geode/Geode.hpp>
#include "../GeneratorConfig.hpp"
using namespace geode::prelude;

class MainPopup : public geode::Popup<LevelEditorLayer*> {
protected:
    bool setup(LevelEditorLayer* editor) override;
public:
    static MainPopup* create(LevelEditorLayer* editor);
private:
    void buildTabBar(CCNode* r);
    void showTab(int i);
    void buildDiffTab();
    void buildLengthStyleTab();
    void buildSongTab();
    CCMenuItemSpriteExtra* makeBtn(const char* lbl,SEL_MenuHandler sel,float w=60.f,float h=24.f);
    CCLabelBMFont* secLbl(const char* t,float x,float y,CCNode* p);
    void highlight(CCArray* a,int tag);
    void onTab(CCObject*);
    void onDifficulty(CCObject*);
    void onLength(CCObject*);
    void onBiome(CCObject*);
    void onMode(CCObject*);
    void onDecoPlus(CCObject*);
    void onDecoMinus(CCObject*);
    void onSong(CCObject*);
    void onRandomSeed(CCObject*);
    void onGenerate(CCObject*);
    void refreshDeco();
    void refreshSeed();
    LevelEditorLayer* m_editor=nullptr;
    GeneratorConfig   m_cfg;
    CCNode*  m_tabs[3]={nullptr,nullptr,nullptr};
    int      m_activeTab=0;
    CCArray* m_diffBtns=nullptr,*m_lenBtns=nullptr,*m_biomeBtns=nullptr;
    CCArray* m_modeBtns=nullptr,*m_songBtns=nullptr;
    CCLabelBMFont* m_decoLbl=nullptr,*m_seedLbl=nullptr;
};
