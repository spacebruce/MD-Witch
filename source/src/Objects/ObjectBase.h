#pragma once
#include <genesis.h>

struct ObjectBase
{
    s32 x;
    s32 y;
    Sprite* spr;
    Vect2D_s16 spriteOffset;
};

void ObjectUpdateSprite(struct ObjectBase* Object, s32 CameraX, s32 CameraY);