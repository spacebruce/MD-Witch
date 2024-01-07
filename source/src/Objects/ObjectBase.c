#include "ObjectBase.h"

void ObjectUpdateSprite(struct ObjectBase* Object, s32 CameraX, s32 CameraY)
{
    if(Object->spr != NULL)
    {
        SPR_setPosition(Object->spr, (Object->x + Object->spriteOffset.x) - CameraX, (Object->y + Object->spriteOffset.y) - CameraY);
    }
}