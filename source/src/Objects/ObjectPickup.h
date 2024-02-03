#pragma once
#include "ObjectBase.h"

#include "../defines.h"
#include <genesis.h>

typedef struct ObjectPickup
{
    struct ObjectBase Base;
} ObjectPickup;

void ObjectPickupInit(void* object);
void ObjectPickupUpdate(void* object);
void ObjectPickupKill(void* object);