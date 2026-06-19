#pragma once
#include "../GeneratorConfig.hpp"
#include "../IDs.hpp"
#include "Pattern.hpp"
#include <cmath>
#include <algorithm>

class DifficultyEngine {
public:
    explicit DifficultyEngine(const GeneratorConfig& cfg) : m_cfg(cfg) {}

    float localDiff(float t) const {
        float base=m_cfg.diffNorm();
        if(t<0.15f) return base*(0.35f+0.65f*(t/0.15f));
        float c=0.f,r=0.f;
        if(t>=0.68f&&t<=0.82f) c=std::sin((t-0.68f)/0.14f*3.14159f)*0.15f;
        if(t>=0.82f&&t<=0.88f) r=-0.12f*std::sin((t-0.82f)/0.06f*3.14159f);
        if(t>0.88f) return std::min(1.f,base+0.08f);
        return std::clamp(base+c+r,0.f,1.f);
    }
    int   minDiffInt(float t)  const { return static_cast<int>(localDiff(t)*8.f); }
    float gap(float t)         const { return m_cfg.minGap()+(1.f-localDiff(t))*50.f; }

    PatternMode modeForSection(int idx, int total) const {
        switch(m_cfg.modeProfile){
            case ModeProfile::CubeOnly:  return PatternMode::Cube;
            case ModeProfile::ShipHeavy: return (idx%3==0)?PatternMode::Cube:PatternMode::Ship;
            case ModeProfile::WaveHeavy: return (idx%4==0)?PatternMode::Cube:PatternMode::Wave;
            case ModeProfile::BallUFO:   return (idx%2==0)?PatternMode::Ball:PatternMode::UFO;
            case ModeProfile::Mixed: {
                const PatternMode c[]={PatternMode::Cube,PatternMode::Ship,PatternMode::Cube,
                    PatternMode::Wave,PatternMode::Cube,PatternMode::Ball,
                    PatternMode::Cube,PatternMode::UFO};
                return c[idx%8];
            }
            case ModeProfile::Insanity: {
                const PatternMode c[]={PatternMode::Cube,PatternMode::Ship,PatternMode::Wave,
                    PatternMode::Ball,PatternMode::UFO};
                return c[idx%5];
            }
        }
        return PatternMode::Cube;
    }

    int speedPortalId(float t) const {
        float d=localDiff(t);
        if(d<0.25f) return GDID::SPEED_NORMAL;
        if(d<0.50f) return GDID::SPEED_DOUBLE;
        if(d<0.75f) return GDID::SPEED_TRIPLE;
        return GDID::SPEED_QUAD;
    }
private:
    const GeneratorConfig& m_cfg;
};
