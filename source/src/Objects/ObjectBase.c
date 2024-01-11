#include "ObjectBase.h"

void ObjectUpdateSprite(struct ObjectBase* Object, const fix32 CameraX, const fix32 CameraY)
{
    if(Object->spr != NULL)
    {
        s16 x = fix32ToInt(fix32Sub(Object->x, CameraX));
        s16 y = fix32ToInt(fix32Sub(Object->y, CameraY));
        SPR_setPosition(Object->spr, x + Object->spriteOffset.x, y + Object->spriteOffset.y);
    }
}