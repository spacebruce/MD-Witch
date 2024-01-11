#include "ObjectPickup.h"

#include <resources.h>

void ObjectPickupInit(ObjectPickup* object)
{
    object->Base.spr = SPR_addSprite(&gfx_cursor, 0,0, TILE_ATTR(PAL_PLAYER, 0, false, false));
    SPR_setVisibility(object->Base.spr, AUTO_FAST);
}
void ObjectPickupUpdate(ObjectPickup *object)
{
    object->Base.x = fix32Add(object->Base.x, FIX32(2));
    if(fix32ToInt(object->Base.x) > 256)
        object->Base.x = FIX32(-8);
}