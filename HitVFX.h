#pragma once
//#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class HitVFX
{
public:
    HitVFX();
    void CreatePosition(float x, float y, float z);
private:
    EmitterData data;
};

