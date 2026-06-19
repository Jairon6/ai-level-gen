#pragma once
#include "Pattern.hpp"
#include <vector>

class PatternLibrary {
public:
    static PatternLibrary& get();
    std::vector<const Pattern*> query(int diffInt, PatternMode mode) const;
    const std::vector<Pattern>& decoPatterns()  const { return m_deco; }
    const std::vector<Pattern>& transitions()   const { return m_transitions; }
    size_t size() const { return m_gameplay.size(); }
private:
    PatternLibrary();
    void buildCubePatterns();
    void buildShipPatterns();
    void buildWavePatterns();
    void buildBallPatterns();
    void buildUFOPatterns();
    void buildDecoPatterns();
    void buildTransitions();
    std::vector<Pattern> m_gameplay, m_deco, m_transitions;
};
