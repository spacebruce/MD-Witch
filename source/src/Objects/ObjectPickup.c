#include "ObjectPickup.h"

#include <resources.h>

void ObjectPickupInit(ObjectPickup* object)
{
    object->Base.spr = SPR_addSprite(&gfx_cursor, 0,0, TILE_ATTR(PAL_PLAYER, 0, false, false));
    SPR_setVisibility(object->Base.spr, AUTO_FAST);
}
void ObjectPickupUpdate(ObjectPickup *object)
{
    object->Base.x = ((object->Base.x + 2) % 256);
}