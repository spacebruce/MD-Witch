#pragma once
#include <genesis.h>

#include "../Types/Collision.h"

// Needs optimising
struct ObjectBase
{
    struct CollisionObject Collision;
    fix32 x, y;                 //  True coordinates
    int16_t StartX, StartY;     //  lower res
    Sprite* spr;
    Vect2D_s16 spriteOffset;
    // Bookkeeping
    uint8_t ActiveObjectID;
    int ObjectType;
};

void ObjectBaseInit(struct ObjectBase* Object, fix32 x, fix32 y, int ObjectType);
void ObjectSetPositionFix32(struct ObjectBase* Object, fix32 X, fix32 Y);
void ObjectSetPositionS32(struct ObjectBase* Object, s32 X, s32 Y);
void ObjectUpdateSprite(struct ObjectBase* Object, fix32 CameraX, fix32 CameraY);