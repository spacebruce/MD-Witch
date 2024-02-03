#include "ObjectBase.h"


void ObjectBaseInit(struct ObjectBase* Object, fix32 x, fix32 y, int ObjectType)
{
    Object->Flags = 0x0000;
    Object->StartX = fix32ToInt(x);
    Object->StartY = fix32ToInt(y);
    ObjectSetPositionFix32(Object, x, y);
    Object->ObjectType = ObjectType;
}

void ObjectSetPositionFix32(struct ObjectBase* Object, const fix32 X, const fix32 Y)
{
    Object->x = X;
    Object->y = Y;
}
void ObjectSetPositionS32(struct ObjectBase* Object, const s32 X, const s32 Y)
{
    Object->x = FIX32(X);
    Object->y = FIX32(Y);
}

void ObjectUpdateSprite(struct ObjectBase* Object, const fix32 CameraX, const fix32 CameraY)
{
    if(Object->spr != NULL)
    {
        s16 x = fix32ToInt(fix32Sub(Object->x, CameraX));
        s16 y = fix32ToInt(fix32Sub(Object->y, CameraY));
        SPR_setPosition(Object->spr, x + Object->spriteOffset.x, y + Object->spriteOffset.y);
    }
}