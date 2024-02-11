#include "HitboxGroup.h"

void InitHitboxGroup(struct HitboxGroup* Group, const u16 Capacity)
{
    u16 SizeNeeded = sizeof(struct CollisionObject) * Capacity;
    Group->Objects = MEM_alloc(SizeNeeded);
    if(Group->Objects == NULL)
    {
        return;
    }
    else
    {
        Group->Capacity = Capacity;
    }
}

