#pragma once
#include <cstdint>
#include <string>
#include <algorithm>

enum class Difficulty : int {
    Normal=0,Hard=1,Harder=2,Insane=3,
    EasyDemon=4,MediumDemon=5,HardDemon=6,InsaneDemon=7,ExtremeDemon=8
};
enum class LevelLength : int {
    Tiny=0,Short=1,Medium=2,Long=3,XL=4,Marathon=5,Nightmare=6
};
enum class Biome : int {
    Classic=0,Neon=1,Dark=2,Galaxy=3,Lava=4,Ice=5,Nature=6,Minimal=7
};
enum class ModeProfile : int {
    CubeOnly=0,ShipHeavy=1,WaveHeavy=2,Mixed=3,BallUFO=4,Insanity=5
};
enum class SongChoice : int {
    Electroman=0,Geometrical=1,Theory=2,ClubStep=3,Deadlocked=4,Custom=5
};

struct GeneratorConfig {
    Difficulty   difficulty   = Difficulty::HardDemon;
    LevelLength  length       = LevelLength::Long;
    Biome        biome        = Biome::Neon;
    ModeProfile  modeProfile  = ModeProfile::Mixed;
    SongChoice   song         = SongChoice::Electroman;
    int          customSongId = 0;
    uint32_t     seed         = 1337;
    int          decoLevel    = 70;
    bool         useTriggers  = true;

    float totalWidth() const {
        switch(length){
            case LevelLength::Tiny:     return 400.f;
            case LevelLength::Short:    return 1000.f;
            case LevelLength::Medium:   return 2000.f;
            case LevelLength::Long:     return 4000.f;
            case LevelLength::XL:       return 8000.f;
            case LevelLength::Marathon: return 20000.f;
            case LevelLength::Nightmare:return 60000.f;
        }
        return 2000.f;
    }
    float diffNorm() const { return static_cast<float>(static_cast<int>(difficulty))/8.f; }
    float minGap()   const { return 60.f - diffNorm()*50.f; }
    int   sections() const { return std::max(static_cast<int>(totalWidth()/800.f)+1,2); }
    int songId() const {
        switch(song){
            case SongChoice::Electroman: return 13;
            case SongChoice::Geometrical:return 22;
            case SongChoice::Theory:     return 55;
            case SongChoice::ClubStep:   return 25;
            case SongChoice::Deadlocked: return 63;
            case SongChoice::Custom:     return customSongId;
        }
        return 13;
    }
    std::string difficultyName() const {
        const char* n[]={"Normal","Hard","Harder","Insane","Easy Demon","Medium Demon","Hard Demon","Insane Demon","Extreme Demon"};
        int i=static_cast<int>(difficulty);
        return (i>=0&&i<=8)?n[i]:"?";
    }
    std::string lengthName() const {
        const char* n[]={"Tiny","Short","Medium","Long","XL","Marathon","Nightmare"};
        int i=static_cast<int>(length);
        return (i>=0&&i<=6)?n[i]:"?";
    }
    std::string biomeName() const {
        const char* n[]={"Classic","Neon","Dark","Galaxy","Lava","Ice","Nature","Minimal"};
        int i=static_cast<int>(biome);
        return (i>=0&&i<=7)?n[i]:"?";
    }
};
