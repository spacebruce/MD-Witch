#include "ObjectPickup.h"

#include <resources.h>

void ObjectPickupInit(void* object)
{
    ObjectPickup* Pickup = (ObjectPickup*)object;
    Pickup->Base.spr = SPR_addSprite(&gfx_cursor, 0,0, TILE_ATTR(PAL_PLAYER, 0, false, false));
    SPR_setVisibility(Pickup->Base.spr, AUTO_FAST);

    Pickup->Base.Collision.Type = CT_Circle;
    Pickup->Base.Collision.Collision.Circle.Radius = 4;
}

void ObjectPickupUpdate(void* object)
{
    ObjectPickup* Pickup = (ObjectPickup*)object;
    Pickup->Base.x = fix32Add(Pickup->Base.x, FIX32(2));
    if(fix32ToInt(Pickup->Base.x) > 256)
        Pickup->Base.x = FIX32(-8);
    
    SetCollisionRectangle(&Pickup->Base.Collision, fix32ToInt(Pickup->Base.x), fix32ToInt(Pickup->Base.y), 8,8);
    //SetCollisionCircle(object->Base.Collision, fix32ToInt(object->Base.x), fix32ToInt(object->Base.y), 4);
}