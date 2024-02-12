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

struct CollisionObject *collidedObjects[MAX_COLLISIONS_RETURN + 1]; 

struct CollisionObject **HitboxGroupCheckCollision(struct HitboxGroup *group, struct CollisionObject *newObject)
{
    int collidedCount = 0;

    for (int i = 0; i < group->Capacity; i++)
    {
        if (CheckCollision(group->Objects[i], newObject))
        {
            if (collidedCount < MAX_COLLISIONS_RETURN)
            {
                collidedObjects[collidedCount++] = group->Objects[i];
            } else 
            {
                break;
            }
        }
    }

    // Null-terminate the list
    collidedObjects[collidedCount] = NULL;

    return collidedObjects;
}