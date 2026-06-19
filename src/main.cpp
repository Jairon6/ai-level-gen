#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "ui/MainPopup.hpp"
using namespace geode::prelude;

class $modify(AIGenEditorUI, EditorUI) {
    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) return false;
        auto* btnBg = CCScale9Sprite::create("GJ_button_05.png");
        if (!btnBg) btnBg = CCScale9Sprite::create("GJ_button_01.png");
        btnBg->setContentSize({76.f, 28.f});
        auto* lbl = CCLabelBMFont::create("AI Gen", "bigFont.fnt");
        lbl->setScale(0.50f);
        lbl->setColor({255, 230, 80});
        lbl->setPosition({38.f, 14.f});
        btnBg->addChild(lbl);
        auto* btn = CCMenuItemSpriteExtra::create(btnBg, this, menu_selector(AIGenEditorUI::onAIGen));
        auto ws = CCDirector::sharedDirector()->getWinSize();
        auto* menu = CCMenu::create(btn, nullptr);
        menu->setPosition({ws.width - 50.f, ws.height - 58.f});
        menu->setZOrder(100);
        this->addChild(menu);
        log::info("[AILevelGen] Mod loaded — AI Gen button added to editor.");
        return true;
    }
    void onAIGen(CCObject*) {
        if (!m_editorLayer) { log::error("[AILevelGen] No editor layer."); return; }
        MainPopup::create(m_editorLayer)->show();
    }
};
