#include "HitboxGroup.h"

void InitHitboxGroup(struct HitboxGroup* Group, const u16 Capacity)
{
    u16 SizeNeeded = sizeof(struct CollisionObject*) * Capacity;
    Group->Objects = MEM_alloc(SizeNeeded);
    Group->Insert = 0;
    if(Group->Objects == NULL)
    {
        return;
    }
    else
    {
        Group->Capacity = Capacity;
    }
}

void HitboxGroupRemoveCollider(struct HitboxGroup *group, struct CollisionObject *object) 
{
    for(u16 i = 0; i < group->Capacity; ++i)
    {
        if(group->Objects[i] == object)
        {
            group->Objects[i] = NULL;
            if(i < group->Insert)
                group->Insert = i;
        }
    }
}

bool HitboxGroupRegisterCollider(struct HitboxGroup *group, struct CollisionObject *object)
{
    if(group->Count >= group->Capacity)
        return false;
    for(u16 i = group->Insert; i < group->Capacity; ++i)
    {
        // OK
        if(group->Objects[i] == NULL)
        {
            group->Objects[i] = object;
            group->Insert = (i + 1);
            return true;
            break;
        }
    }
    return false;
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