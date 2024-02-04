#include "ObjectPickup.h"

#include <resources.h>

void ObjectPickupInit(void* object)
{
    ObjectPickup* Pickup = (ObjectPickup*)object;

    Pickup->Base.spr = SPR_addSprite(&sprCursor, Pickup->Base.StartX, Pickup->Base.StartY, TILE_ATTR(PAL_PLAYER, 1, false, false));
    //SPR_setVisibility(Pickup->Base.spr, AUTO_FAST);

    Pickup->Base.Collision.Type = CT_Circle;
    Pickup->Base.Collision.Collision.Circle.Radius = 4;
}

void ObjectPickupUpdate(void* object)
{
    ObjectPickup* Pickup = (ObjectPickup*)object;

    //Pickup->Base.x = FIX32(32);
    //Pickup->Base.y = FIX32(32);
    Pickup->Base.x = (Pickup->Base.x + FIX32(2));
    if(Pickup->Base.x > FIX32(256))
    {
        Pickup->Base.Flags |= OBJECT_DESTROY;
        Pickup->Base.x = FIX32(0);
    }
    
    SetCollisionRectangle(&Pickup->Base.Collision, fix32ToInt(Pickup->Base.x), fix32ToInt(Pickup->Base.y), 8,8);
    //SetCollisionCircle(object->Base.Collision, fix32ToInt(object->Base.x), fix32ToInt(object->Base.y), 4);
}

void ObjectPickupFree(void* object)
{
    SPR_releaseSprite(((struct ObjectBase*)object)->spr);
    SPR_defragVRAM();
}