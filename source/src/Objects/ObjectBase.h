#pragma once
#include <genesis.h>

struct ObjectBase
{
    fix32 x;
    fix32 y;
    Sprite* spr;
    Vect2D_s16 spriteOffset;
};

void ObjectUpdateSprite(struct ObjectBase* Object, fix32 CameraX, fix32 CameraY);