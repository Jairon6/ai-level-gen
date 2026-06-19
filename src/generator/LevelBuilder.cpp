#include "LevelBuilder.hpp"
#include "../IDs.hpp"
#include <cmath>
#include <algorithm>
using namespace GDID;

LevelBuilder::LevelBuilder(const GeneratorConfig& cfg)
    : m_cfg(cfg), m_diff(cfg), m_rng(cfg.seed) {}

void LevelBuilder::build(LevelEditorLayer* editor, ProgressCallback onProgress){
    float endX = X_START + m_cfg.totalWidth();
    log::info("[AILevelGen] Building: diff={} len={} biome={} seed={}",
        m_cfg.difficultyName(), m_cfg.lengthName(), m_cfg.biomeName(), m_cfg.seed);
    fillGroundAndCeiling(editor, endX);
    if(onProgress) onProgress(0.05f);
    buildSections(editor, [&](float p){ if(onProgress) onProgress(0.05f+p*0.80f); });
    placeDecoration(editor, endX);
    if(onProgress) onProgress(0.95f);
    placeSpeedPortals(editor, endX);
    if(onProgress) onProgress(1.00f);
    log::info("[AILevelGen] Done. Level is {:.0f} units wide.", endX);
}

void LevelBuilder::fillGroundAndCeiling(LevelEditorLayer* editor, float endX){
    for(float x=0.f; x<=endX+BLOCK; x+=BLOCK){
        placeObj(BLOCK_1x1, x, Y_GND, 0.f, editor);
        placeObj(BLOCK_1x1, x, Y_CEIL+BLOCK, 0.f, editor);
    }
}

void LevelBuilder::buildSections(LevelEditorLayer* editor, ProgressCallback cb){
    int   total  = m_cfg.sections();
    float sw     = m_cfg.totalWidth() / total;
    float curX   = X_START;
    PatternMode prev = PatternMode::Cube;

    for(int sec=0; sec<total; ++sec){
        float t   = static_cast<float>(sec)/total;
        float end = X_START + (sec+1)*sw;
        int   d   = m_diff.minDiffInt(t);
        PatternMode mode = m_diff.modeForSection(sec, total);

        if(mode!=prev && sec>0){
            placeTransition(prev, mode, curX, editor);
            curX += 90.f;
        }
        prev = mode;

        placeObj(m_diff.speedPortalId(t), curX, Y_SP1, 0.f, editor);
        curX += 60.f;

        while(curX < end-30.f){
            const Pattern* pat = pickPattern(d, mode);
            if(!pat) break;
            placePattern(*pat, curX, editor);
            curX += pat->width + m_diff.gap(t);
        }
        curX = std::max(curX, end);
        if(cb) cb(static_cast<float>(sec+1)/total);
    }
}

void LevelBuilder::placeDecoration(LevelEditorLayer* editor, float endX){
    if(m_cfg.biome==Biome::Minimal) return;
    float density = m_cfg.decoLevel/100.f;
    std::uniform_real_distribution<float> sp(BLOCK*3.f, BLOCK*3.f+(1.f-density)*BLOCK*8.f);
    float curX = X_START;
    while(curX < endX){
        const Pattern* d = pickDecoPat();
        if(!d){ curX += BLOCK*4.f; continue; }
        placePattern(*d, curX, editor);
        curX += d->width + sp(m_rng);
    }
}

void LevelBuilder::placeSpeedPortals(LevelEditorLayer* editor, float endX){
    float sw = m_cfg.totalWidth()/m_cfg.sections();
    for(int i=0; i<m_cfg.sections(); ++i){
        float t = static_cast<float>(i)/m_cfg.sections();
        placeObj(m_diff.speedPortalId(t), X_START+i*sw, Y_SP1-5.f, 0.f, editor);
    }
}

void LevelBuilder::placeTransition(PatternMode /*from*/, PatternMode to,
                                    float x, LevelEditorLayer* editor){
    int id;
    switch(to){
        case PatternMode::Ship: id=MODE_SHIP; break;
        case PatternMode::Wave: id=MODE_WAVE; break;
        case PatternMode::Ball: id=MODE_BALL; break;
        case PatternMode::UFO:  id=MODE_UFO;  break;
        default:                id=MODE_CUBE; break;
    }
    placeObj(id, x, Y_SP1, 0.f, editor);
}

void LevelBuilder::placePattern(const Pattern& pat, float anchorX, LevelEditorLayer* editor){
    for(const auto& obj : pat.objects)
        placeObj(obj.id, anchorX+obj.dx, obj.y, obj.rotation, editor);
}

void LevelBuilder::placeObj(int id, float x, float y, float rot, LevelEditorLayer* editor){
    auto* obj = editor->createObject(id, {x,y}, false);
    if(!obj) return;
    if(rot!=0.f){
        obj->setRotation(rot);
        editor->updateObjectOrientationForAnim(obj);
    }
}

const Pattern* LevelBuilder::pickPattern(int diffInt, PatternMode mode){
    auto cands = PatternLibrary::get().query(diffInt, mode);
    if(cands.empty()) cands = PatternLibrary::get().query(diffInt, PatternMode::Any);
    if(cands.empty()) return nullptr;
    std::vector<int> w;
    w.reserve(cands.size());
    for(const auto* p : cands) w.push_back(1+p->minDiffInt);
    std::discrete_distribution<int> dist(w.begin(), w.end());
    return cands[dist(m_rng)];
}

const Pattern* LevelBuilder::pickDecoPat(){
    const auto& d = PatternLibrary::get().decoPatterns();
    if(d.empty()) return nullptr;
    std::uniform_int_distribution<int> dist(0, static_cast<int>(d.size())-1);
    return &d[dist(m_rng)];
}
