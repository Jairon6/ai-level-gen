#include "MainPopup.hpp"
#include "../generator/LevelBuilder.hpp"
#include <Geode/Geode.hpp>
#include <chrono>
using namespace geode::prelude;
static constexpr float PW=400.f,PH=340.f;
static const ccColor3B GOLD={255,215,0},GREY={160,160,160};

MainPopup* MainPopup::create(LevelEditorLayer* e){
    auto* p=new MainPopup();
    if(p&&p->initAnchored(PW,PH,e)){p->autorelease();return p;}
    CC_SAFE_DELETE(p);return nullptr;
}

bool MainPopup::setup(LevelEditorLayer* editor){
    m_editor=editor; m_cfg={};
    m_diffBtns=CCArray::create(); m_diffBtns->retain();
    m_lenBtns=CCArray::create();  m_lenBtns->retain();
    m_biomeBtns=CCArray::create();m_biomeBtns->retain();
    m_modeBtns=CCArray::create(); m_modeBtns->retain();
    m_songBtns=CCArray::create(); m_songBtns->retain();
    auto* r=m_mainLayer;
    auto* t=CCLabelBMFont::create("AI Level Generator","goldFont.fnt");
    t->setScale(0.70f);t->setPosition({PW*.5f,PH-18.f});r->addChild(t);
    buildTabBar(r);
    for(int i=0;i<3;++i){m_tabs[i]=CCNode::create();m_tabs[i]->setPosition({0,0});m_tabs[i]->setVisible(false);r->addChild(m_tabs[i]);}
    buildDiffTab();buildLengthStyleTab();buildSongTab();showTab(0);
    auto* gm=CCMenu::create();gm->setPosition({0,0});r->addChild(gm,5);
    auto* gb=makeBtn("GENERATE LEVEL",menu_selector(MainPopup::onGenerate),160.f,34.f);
    gb->setPosition({PW*.5f,28.f});gm->addChild(gb);
    auto* sh=CCLabelBMFont::create("Seed:","bigFont.fnt");sh->setScale(0.38f);sh->setAnchorPoint({0,.5f});sh->setPosition({14.f,60.f});r->addChild(sh);
    char buf[16];snprintf(buf,sizeof(buf),"%u",m_cfg.seed);
    m_seedLbl=CCLabelBMFont::create(buf,"bigFont.fnt");m_seedLbl->setScale(0.38f);m_seedLbl->setAnchorPoint({0,.5f});m_seedLbl->setPosition({70.f,60.f});r->addChild(m_seedLbl);
    auto* sm=CCMenu::create();sm->setPosition({0,0});r->addChild(sm,5);
    auto* rb=makeBtn("Random",menu_selector(MainPopup::onRandomSeed),58.f,22.f);rb->setPosition({230.f,60.f});sm->addChild(rb);
    return true;
}

void MainPopup::buildTabBar(CCNode* r){
    auto* m=CCMenu::create();m->setPosition({0,0});r->addChild(m,4);
    const char* l[]={"Difficulty","Length & Style","Song"};
    float xs[]={70.f,200.f,330.f};
    for(int i=0;i<3;++i){
        auto* bg=CCScale9Sprite::create("GJ_button_02.png");bg->setContentSize({115.f,26.f});
        auto* lbl=CCLabelBMFont::create(l[i],"bigFont.fnt");lbl->setScale(0.36f);lbl->setPosition({57.f,13.f});bg->addChild(lbl);
        auto* btn=CCMenuItemSpriteExtra::create(bg,this,menu_selector(MainPopup::onTab));
        btn->setTag(i);btn->setPosition({xs[i],PH-45.f});m->addChild(btn);
    }
}

void MainPopup::showTab(int i){for(int j=0;j<3;++j)if(m_tabs[j])m_tabs[j]->setVisible(j==i);m_activeTab=i;}

void MainPopup::buildDiffTab(){
    CCNode* t=m_tabs[0];auto* m=CCMenu::create();m->setPosition({0,0});t->addChild(m);
    secLbl("DIFFICULTY",14.f,PH-80.f,t);
    const char* dl[]={"Normal","Hard","Harder","Insane","Easy\nDmn","Med\nDmn","Hard\nDmn","Ins\nDmn","Ext\nDmn"};
    float xs[]={26,68,110,152,194,236,278,320,362};
    for(int i=0;i<9;++i){
        auto* bg=CCScale9Sprite::create("GJ_button_04.png");bg->setContentSize({38.f,38.f});
        auto* lb=CCLabelBMFont::create(dl[i],"chatFont.fnt");lb->setScale(0.34f);lb->setPosition({19.f,19.f});bg->addChild(lb);
        auto* b=CCMenuItemSpriteExtra::create(bg,this,menu_selector(MainPopup::onDifficulty));
        b->setTag(i);b->setPosition({xs[i],PH-115.f});m->addChild(b);m_diffBtns->addObject(b);
    }
    highlight(m_diffBtns,static_cast<int>(m_cfg.difficulty));
    secLbl("GAME MODE",14.f,PH-160.f,t);
    const char* ml[]={"Cube Only","Ship Heavy","Wave Heavy","Mixed","Ball+UFO","Insanity"};
    float mxs[]={40.f,112.f,184.f,256.f,328.f,200.f},mys[]={PH-190.f,PH-190.f,PH-190.f,PH-190.f,PH-190.f,PH-220.f};
    for(int i=0;i<6;++i){
        auto* b=makeBtn(ml[i],menu_selector(MainPopup::onMode),68.f,24.f);
        b->setTag(i);b->setPosition({mxs[i],mys[i]});m->addChild(b);m_modeBtns->addObject(b);
    }
    highlight(m_modeBtns,static_cast<int>(m_cfg.modeProfile));
}

void MainPopup::buildLengthStyleTab(){
    CCNode* t=m_tabs[1];auto* m=CCMenu::create();m->setPosition({0,0});t->addChild(m);
    secLbl("LENGTH",14.f,PH-80.f,t);
    const char* ll[]={"Tiny","Short","Medium","Long","XL","Marathon","Nightmare"};
    float lxs[]={30,75,120,165,210,260,330};
    for(int i=0;i<7;++i){
        auto* b=makeBtn(ll[i],menu_selector(MainPopup::onLength),58.f,24.f);
        b->setTag(i);b->setPosition({lxs[i],PH-110.f});m->addChild(b);m_lenBtns->addObject(b);
    }
    highlight(m_lenBtns,static_cast<int>(m_cfg.length));
    secLbl("BIOME",14.f,PH-145.f,t);
    const char* bl[]={"Classic","Neon","Dark","Galaxy","Lava","Ice","Nature","Minimal"};
    float bxs[]={26,76,126,176,226,276,326,376};
    for(int i=0;i<8;++i){
        auto* b=makeBtn(bl[i],menu_selector(MainPopup::onBiome),46.f,22.f);
        b->setTag(i);b->setPosition({bxs[i],PH-170.f});m->addChild(b);m_biomeBtns->addObject(b);
    }
    highlight(m_biomeBtns,static_cast<int>(m_cfg.biome));
    secLbl("DECORATION DENSITY",14.f,PH-205.f,t);
    auto* mn=makeBtn("-",menu_selector(MainPopup::onDecoMinus),28.f,24.f);mn->setPosition({110.f,PH-225.f});m->addChild(mn);
    char db[8];snprintf(db,sizeof(db),"%d%%",m_cfg.decoLevel);
    m_decoLbl=CCLabelBMFont::create(db,"bigFont.fnt");m_decoLbl->setScale(0.45f);m_decoLbl->setPosition({200.f,PH-225.f});t->addChild(m_decoLbl);
    auto* pl=makeBtn("+",menu_selector(MainPopup::onDecoPlus),28.f,24.f);pl->setPosition({290.f,PH-225.f});m->addChild(pl);
}

void MainPopup::buildSongTab(){
    CCNode* t=m_tabs[2];auto* m=CCMenu::create();m->setPosition({0,0});t->addChild(m);
    secLbl("SONG PRESET",14.f,PH-80.f,t);
    const char* sl[]={"Electroman Adv.","Geom. Dominator","Theory of Ev. 2","Club Step","Deadlocked"};
    for(int i=0;i<5;++i){
        auto* b=makeBtn(sl[i],menu_selector(MainPopup::onSong),140.f,26.f);
        b->setTag(i);b->setPosition({(i%2==0)?110.f:290.f,PH-110.f-(i/2)*34.f});
        m->addChild(b);m_songBtns->addObject(b);
    }
    highlight(m_songBtns,static_cast<int>(m_cfg.song));
}

CCMenuItemSpriteExtra* MainPopup::makeBtn(const char* lbl,SEL_MenuHandler sel,float w,float h){
    auto* bg=CCScale9Sprite::create("GJ_button_04.png");bg->setContentSize({w,h});
    auto* l=CCLabelBMFont::create(lbl,"bigFont.fnt");
    float sc=std::min(0.44f,(w-8.f)/(strlen(lbl)*14.f+1.f));
    l->setScale(sc);l->setPosition({w*.5f,h*.5f});bg->addChild(l);
    return CCMenuItemSpriteExtra::create(bg,this,sel);
}

CCLabelBMFont* MainPopup::secLbl(const char* txt,float x,float y,CCNode* p){
    auto* l=CCLabelBMFont::create(txt,"bigFont.fnt");l->setScale(0.38f);l->setColor({220,180,80});
    l->setAnchorPoint({0.f,.5f});l->setPosition({x,y});p->addChild(l);return l;
}

void MainPopup::highlight(CCArray* a,int tag){
    for(unsigned i=0;i<a->count();++i){
        auto* b=static_cast<CCMenuItemSpriteExtra*>(a->objectAtIndex(i));
        b->setColor(b->getTag()==tag?GOLD:GREY);
    }
}

void MainPopup::refreshDeco(){char b[8];snprintf(b,sizeof(b),"%d%%",m_cfg.decoLevel);m_decoLbl->setString(b);}
void MainPopup::refreshSeed(){char b[16];snprintf(b,sizeof(b),"%u",m_cfg.seed);m_seedLbl->setString(b);}

void MainPopup::onTab(CCObject* s){showTab(static_cast<CCNode*>(s)->getTag());}
void MainPopup::onDifficulty(CCObject* s){int t=static_cast<CCNode*>(s)->getTag();m_cfg.difficulty=static_cast<Difficulty>(t);highlight(m_diffBtns,t);}
void MainPopup::onLength(CCObject* s){int t=static_cast<CCNode*>(s)->getTag();m_cfg.length=static_cast<LevelLength>(t);highlight(m_lenBtns,t);}
void MainPopup::onBiome(CCObject* s){int t=static_cast<CCNode*>(s)->getTag();m_cfg.biome=static_cast<Biome>(t);highlight(m_biomeBtns,t);}
void MainPopup::onMode(CCObject* s){int t=static_cast<CCNode*>(s)->getTag();m_cfg.modeProfile=static_cast<ModeProfile>(t);highlight(m_modeBtns,t);}
void MainPopup::onDecoPlus(CCObject*){m_cfg.decoLevel=std::min(100,m_cfg.decoLevel+10);refreshDeco();}
void MainPopup::onDecoMinus(CCObject*){m_cfg.decoLevel=std::max(0,m_cfg.decoLevel-10);refreshDeco();}
void MainPopup::onSong(CCObject* s){int t=static_cast<CCNode*>(s)->getTag();m_cfg.song=static_cast<SongChoice>(t);highlight(m_songBtns,t);}
void MainPopup::onRandomSeed(CCObject*){
    auto now=std::chrono::system_clock::now().time_since_epoch().count();
    m_cfg.seed=static_cast<uint32_t>(now&0xFFFFFFFFu);refreshSeed();
}
void MainPopup::onGenerate(CCObject*){
    if(!m_editor){FLAlertLayer::create("Error","No editor layer.","OK")->show();return;}
    Mod::get()->setSavedValue<int>("last-seed",static_cast<int>(m_cfg.seed));
    LevelBuilder builder(m_cfg);
    builder.build(m_editor);
    FLAlertLayer::create("AI Level Generator",
        ("Level generated!\nDifficulty: <cy>"+m_cfg.difficultyName()+"</c>\nLength: <cg>"+m_cfg.lengthName()+"</c>\nBiome: <cp>"+m_cfg.biomeName()+"</c>").c_str(),
        "Awesome!")->show();
    this->onClose(nullptr);
}
