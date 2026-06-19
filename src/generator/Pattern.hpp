#pragma once
#include <vector>
#include <string>

enum class PatternMode : int { Cube=0,Ship=1,Wave=2,Ball=3,UFO=4,Any=5 };

struct ObjEntry {
    int   id;
    float dx, y, rotation;
    int   editorLayer;
    bool  isDecor, isTrigger;
};

struct Pattern {
    std::string           name;
    float                 width;
    int                   minDiffInt;
    PatternMode           mode;
    bool                  isBoss;
    std::vector<ObjEntry> objects;
};
