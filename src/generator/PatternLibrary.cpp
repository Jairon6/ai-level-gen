#include "PatternLibrary.hpp"
#include "../IDs.hpp"
using namespace GDID;

PatternLibrary& PatternLibrary::get(){ static PatternLibrary i; return i; }

PatternLibrary::PatternLibrary(){
    buildCubePatterns(); buildShipPatterns(); buildWavePatterns();
    buildBallPatterns(); buildUFOPatterns(); buildDecoPatterns(); buildTransitions();
}

std::vector<const Pattern*> PatternLibrary::query(int d,PatternMode m) const {
    std::vector<const Pattern*> r;
    for(const auto& p:m_gameplay)
        if((p.mode==m||p.mode==PatternMode::Any)&&p.minDiffInt<=d) r.push_back(&p);
    return r;
}

void PatternLibrary::buildCubePatterns(){
    m_gameplay.push_back({"C_SingleSpike",60.f,0,PatternMode::Cube,false,{
        {SPIKE_NORMAL,30.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_DoubleSpike",90.f,0,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,45.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_TripleSpike",120.f,2,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,45.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,75.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_PlatformJump",150.f,0,PatternMode::Cube,false,{
        {BLOCK_1x1,30.f,Y_P1,0.f,0,false,false},
        {BLOCK_1x1,60.f,Y_P1,0.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_YellowOrb",120.f,2,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,60.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_CeilSpike",120.f,2,PatternMode::Cube,false,{
        {SPIKE_NORMAL,30.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,60.f,Y_CEIL_SPIKE,180.f,0,false,false}}});
    m_gameplay.push_back({"C_TightSqueeze",90.f,4,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,30.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,60.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_OrbChain",180.f,4,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,60.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,135.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,165.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_AltSaw",210.f,4,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,45.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,75.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,105.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,135.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,165.f,Y_CEIL_SPIKE,180.f,0,false,false}}});
    m_gameplay.push_back({"C_PinkOrbRush",210.f,6,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {ORB_PINK,60.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,75.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SP1,0.f,0,false,false},
        {ORB_PINK,135.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,165.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,180.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_InsaneCorridor",300.f,7,PatternMode::Cube,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,30.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_SMALL,45.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,75.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_SP1,0.f,0,false,false},
        {ORB_PINK,150.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,165.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_SMALL,180.f,Y_SP1,0.f,0,false,false},
        {ORB_BLUE,225.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,255.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"C_ExtremeBoss",360.f,8,PatternMode::Cube,true,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {SPIKE_SMALL,25.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,45.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_SMALL,55.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {ORB_PINK,90.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,105.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {ORB_BLACK,150.f,Y_P1,0.f,0,false,false},
        {BLOCK_HALF,180.f,Y_P1,0.f,0,false,false},
        {SPIKE_NORMAL,195.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,195.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,240.f,Y_SP1,0.f,0,false,false},
        {SPIKE_SMALL,255.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,270.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,300.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,315.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_SMALL,330.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,345.f,Y_SP1,0.f,0,false,false}}});
}

void PatternLibrary::buildShipPatterns(){
    m_gameplay.push_back({"S_BasicTunnel",180.f,0,PatternMode::Ship,false,{
        {BLOCK_1x1,30.f,Y_CEIL+BLOCK,0.f,0,false,false},
        {BLOCK_1x1,60.f,Y_CEIL+BLOCK,0.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"S_NarrowTunnel",240.f,4,PatternMode::Ship,false,{
        {BLOCK_1x1,30.f,Y_SHIP_CENTER+45.f,0.f,0,false,false},
        {BLOCK_1x1,60.f,Y_SHIP_CENTER+45.f,0.f,0,false,false},
        {BLOCK_1x1,90.f,Y_SHIP_CENTER+45.f,0.f,0,false,false},
        {SPIKE_NORMAL,30.f,Y_SHIP_CENTER-45.f,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SHIP_CENTER-45.f,0.f,0,false,false},
        {SPIKE_NORMAL,180.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"S_ExtremeFly",360.f,8,PatternMode::Ship,true,{
        {SPIKE_NORMAL,15.f,Y_SHIP_MIN,0.f,0,false,false},
        {SPIKE_SMALL,25.f,Y_SHIP_MIN,0.f,0,false,false},
        {SPIKE_NORMAL,15.f,Y_SHIP_MAX,180.f,0,false,false},
        {ORB_PINK,75.f,Y_SHIP_CENTER,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SHIP_MIN+30.f,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SHIP_MAX-30.f,180.f,0,false,false},
        {SPIKE_NORMAL,150.f,Y_SHIP_MIN,0.f,0,false,false},
        {ORB_YELLOW,195.f,Y_SHIP_CENTER,0.f,0,false,false},
        {SPIKE_NORMAL,210.f,Y_SHIP_MAX,180.f,0,false,false},
        {SPIKE_SMALL,225.f,Y_SHIP_MIN,0.f,0,false,false},
        {ORB_RED,270.f,Y_SHIP_CENTER,0.f,0,false,false},
        {SPIKE_NORMAL,300.f,Y_SHIP_MIN,0.f,0,false,false},
        {SPIKE_NORMAL,315.f,Y_SHIP_MAX,180.f,0,false,false}}});
}

void PatternLibrary::buildWavePatterns(){
    m_gameplay.push_back({"W_OpenWave",180.f,2,PatternMode::Wave,false,{
        {SPIKE_NORMAL,60.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_NORMAL,60.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_GND_TOP,0.f,0,false,false}}});
    m_gameplay.push_back({"W_ZigZag",300.f,6,PatternMode::Wave,false,{
        {SPIKE_NORMAL,30.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_NORMAL,60.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_SMALL,150.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_SMALL,165.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_NORMAL,195.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_NORMAL,225.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_NORMAL,255.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_NORMAL,270.f,Y_CEIL_BOT,180.f,0,false,false}}});
    m_gameplay.push_back({"W_ExtremeMini",360.f,8,PatternMode::Wave,true,{
        {SPIKE_SMALL,15.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_SMALL,30.f,Y_CEIL_BOT,180.f,0,false,false},
        {BLOCK_HALF,45.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_SMALL,75.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_SMALL,105.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_TINY,120.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_SMALL,150.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_SMALL,180.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_TINY,195.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_TINY,210.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_SMALL,240.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_SMALL,270.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_SMALL,300.f,Y_GND_TOP,0.f,0,false,false},
        {SPIKE_TINY,330.f,Y_CEIL_BOT,180.f,0,false,false},
        {SPIKE_NORMAL,345.f,Y_GND_TOP,0.f,0,false,false}}});
}

void PatternLibrary::buildBallPatterns(){
    m_gameplay.push_back({"B_BasicFlip",150.f,2,PatternMode::Ball,false,{
        {ORB_YELLOW,45.f,Y_P1,0.f,0,false,false},
        {SPIKE_NORMAL,90.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,120.f,Y_P2,0.f,0,false,false}}});
    m_gameplay.push_back({"B_FlipGauntlet",270.f,5,PatternMode::Ball,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,45.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,75.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {ORB_YELLOW,105.f,Y_P1,0.f,0,false,false},
        {SPIKE_NORMAL,135.f,Y_SP1,0.f,0,false,false},
        {ORB_BLUE,165.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,195.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_NORMAL,255.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"B_ExtremeBoss",330.f,8,PatternMode::Ball,true,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {ORB_BLACK,45.f,Y_P1,0.f,0,false,false},
        {SPIKE_NORMAL,60.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_SMALL,75.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,105.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {ORB_BLUE,150.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,165.f,Y_SP1,0.f,0,false,false},
        {ORB_BLACK,210.f,Y_P1,0.f,0,false,false},
        {ORB_PINK,270.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,300.f,Y_SP1,0.f,0,false,false}}});
}

void PatternLibrary::buildUFOPatterns(){
    m_gameplay.push_back({"U_BasicUFO",180.f,2,PatternMode::UFO,false,{
        {SPIKE_NORMAL,30.f,Y_SP1,0.f,0,false,false},
        {SPIKE_NORMAL,30.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {ORB_YELLOW,90.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,120.f,Y_SP1,0.f,0,false,false}}});
    m_gameplay.push_back({"U_OrbHop",240.f,5,PatternMode::UFO,false,{
        {SPIKE_NORMAL,15.f,Y_SP1,0.f,0,false,false},
        {ORB_YELLOW,45.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,75.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {ORB_YELLOW,105.f,Y_P3,0.f,0,false,false},
        {SPIKE_NORMAL,135.f,Y_SP1,0.f,0,false,false},
        {ORB_PINK,165.f,Y_P2,0.f,0,false,false},
        {SPIKE_NORMAL,195.f,Y_CEIL_SPIKE,180.f,0,false,false},
        {SPIKE_SMALL,210.f,Y_SP1,0.f,0,false,false}}});
}

void PatternLibrary::buildDecoPatterns(){
    m_deco.push_back({"D_GroundStrip",60.f,0,PatternMode::Any,false,{
        {BLOCK_HALF,15.f,Y_GND,0.f,1,true,false},
        {BLOCK_HALF,45.f,Y_GND,0.f,1,true,false}}});
    m_deco.push_back({"D_Diamonds",90.f,0,PatternMode::Any,false,{
        {DECO_DIAMOND,30.f,Y_MID,45.f,2,true,false},
        {DECO_DIAMOND,60.f,Y_MID+30.f,45.f,2,true,false}}});
    m_deco.push_back({"D_Stars",120.f,0,PatternMode::Any,false,{
        {DECO_STAR,20.f,Y_MID,30.f,2,true,false},
        {DECO_STAR,50.f,Y_P2,60.f,2,true,false},
        {DECO_STAR,80.f,Y_P3,120.f,2,true,false},
        {DECO_STAR,110.f,Y_MID-20.f,90.f,2,true,false}}});
    m_deco.push_back({"D_Crystals",150.f,0,PatternMode::Any,false,{
        {DECO_CRYSTAL,20.f,Y_P1,0.f,2,true,false},
        {DECO_CRYSTAL,60.f,Y_CEIL_BOT,180.f,2,true,false},
        {DECO_CRYSTAL,90.f,Y_P2,45.f,2,true,false},
        {DECO_CRYSTAL,120.f,Y_P3,90.f,2,true,false}}});
    m_deco.push_back({"D_Flames",90.f,0,PatternMode::Any,false,{
        {DECO_FLAME,15.f,Y_GND_TOP+5.f,0.f,2,true,false},
        {DECO_FLAME,45.f,Y_CEIL_BOT-5.f,180.f,2,true,false},
        {DECO_FLAME,75.f,Y_GND_TOP+5.f,0.f,2,true,false}}});
    m_deco.push_back({"D_Skulls",120.f,0,PatternMode::Any,false,{
        {DECO_SKULL,20.f,Y_P1,0.f,2,true,false},
        {DECO_SKULL,60.f,Y_MID,0.f,2,true,false},
        {DECO_SKULL,100.f,Y_P2,0.f,2,true,false}}});
    m_deco.push_back({"D_Meteors",240.f,0,PatternMode::Any,false,{
        {DECO_METEOR,30.f,Y_P3,210.f,2,true,false},
        {DECO_METEOR,70.f,Y_P4,225.f,2,true,false},
        {DECO_METEOR,110.f,Y_P2,195.f,2,true,false},
        {DECO_METEOR,150.f,Y_MID,215.f,2,true,false},
        {DECO_METEOR,190.f,Y_P3,200.f,2,true,false}}});
    m_deco.push_back({"D_RingChain",180.f,0,PatternMode::Any,false,{
        {DECO_RING,30.f,Y_P1,0.f,2,true,false},
        {DECO_CHAIN,60.f,Y_P2,0.f,2,true,false},
        {DECO_RING,90.f,Y_P3,0.f,2,true,false},
        {DECO_CHAIN,120.f,Y_MID,0.f,2,true,false},
        {DECO_RING,150.f,Y_P2,0.f,2,true,false}}});
}

void PatternLibrary::buildTransitions(){
    m_transitions.push_back({"T_ToShip",60.f,0,PatternMode::Any,false,{{MODE_SHIP,30.f,Y_SP1,0.f,0,false,false}}});
    m_transitions.push_back({"T_ToCube",60.f,0,PatternMode::Any,false,{{MODE_CUBE,30.f,Y_SP1,0.f,0,false,false}}});
    m_transitions.push_back({"T_ToWave",60.f,0,PatternMode::Any,false,{{MODE_WAVE,30.f,Y_SP1,0.f,0,false,false}}});
    m_transitions.push_back({"T_ToBall",60.f,0,PatternMode::Any,false,{{MODE_BALL,30.f,Y_SP1,0.f,0,false,false}}});
    m_transitions.push_back({"T_ToUFO", 60.f,0,PatternMode::Any,false,{{MODE_UFO, 30.f,Y_SP1,0.f,0,false,false}}});
}
