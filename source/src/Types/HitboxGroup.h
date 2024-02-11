#pragma once

#include "Collision.h"

struct HitboxGroup
{
    struct CollisionObject* Objects;
    u16 Capacity;
};

void InitHitboxGroup(struct HitboxGroup* Group, const u16 Capacity);
//bool CheckCollision(struct HitboxGroup* Group, 
