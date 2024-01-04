#include "ObjectPickup.h"

#include <resources.h>

void ObjectPickupInit(ObjectPickup* object)
{
    object->Base.spr = SPR_addSprite(&gfx_cursor, 0,0, TILE_ATTR(PAL_PLAYER, 0, false, false));
}
void ObjectPickupUpdate(ObjectPickup *object)
{

}