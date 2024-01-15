#pragma once
#include <genesis.h>

#include "../Types/Collision.h"

struct ObjectBase
{
    fix32 x;
    fix32 y;
    Sprite* spr;
    Vect2D_s16 spriteOffset;
    CollisionObject Collision;
};

void ObjectSetPositionFix32(struct ObjectBase* Object, fix32 X, fix32 Y);
void ObjectSetPositionS32(struct ObjectBase* Object, s32 X, s32 Y);
void ObjectUpdateSprite(struct ObjectBase* Object, fix32 CameraX, fix32 CameraY);