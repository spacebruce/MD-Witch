#pragma once
#include <genesis.h>

#include "../Types/Collision.h"

typedef enum 
{
    OBJECT_DESTROY = 0B1,
    OBJECT_INVISIBLE = 0B10,
    OBJECT_RESERVED_1 = 0B100,
    OBJECT_RESERVED_2 = 0B1000,
    OBJECT_RESERVED_3 = 0B10000,
    OBJECT_RESERVED_4 = 0B100000,
    OBJECT_RESERVED_5 = 0B1000000,
    OBJECT_RESERVED_6 = 0B10000000,
    OBJECT_RESERVED_7 = 0B100000000,
    OBJECT_RESERVED_8 = 0B1000000000,
    OBJECT_RESERVED_9 = 0B10000000000,
    OBJECT_RESERVED_A = 0B100000000000,
    OBJECT_RESERVED_B = 0B1000000000000,
    OBJECT_RESERVED_C = 0B10000000000000,
    OBJECT_RESERVED_D = 0B100000000000000,
} ObjectFlags;

// Needs optimising
struct ObjectBase
{
    fix32 x, y;                 //  True coordinates
    int16_t StartX, StartY;     //  lower res
    Sprite* spr;
    Vect2D_s16 spriteOffset;
    //
    uint16_t Flags;
    // Bookkeeping
    uint8_t ActiveObjectID;
    int ObjectType;
};
static const int BaseSize = sizeof(struct ObjectBase);

void ObjectBaseInit(struct ObjectBase* Object, fix32 x, fix32 y, int ObjectType);
void ObjectSetPositionFix32(struct ObjectBase* Object, fix32 X, fix32 Y);
void ObjectSetPositionS32(struct ObjectBase* Object, s32 X, s32 Y);
void ObjectUpdateSprite(struct ObjectBase* Object, fix32 CameraX, fix32 CameraY);