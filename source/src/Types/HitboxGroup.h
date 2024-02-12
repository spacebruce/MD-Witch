#pragma once

#include "Collision.h"

#define MAX_COLLISIONS_RETURN (5)

struct HitboxGroup
{
    struct CollisionObject **Objects;
    s16 Count;
    s16 Capacity;
};

void HitboxGroupInit(struct HitboxGroup* Group, const u16 Capacity);
void HitboxGroupRegisterCollider(struct HitboxGroup *group, struct CollisionObject *object);
struct CollisionObject **HitboxGroupCheckCollision(struct HitboxGroup *group, struct CollisionObject *newObject);