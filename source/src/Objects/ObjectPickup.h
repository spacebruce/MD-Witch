#pragma once
#include "ObjectBase.h"

#include "../defines.h"
#include <genesis.h>

typedef struct ObjectPickup
{
    struct ObjectBase Base;

} ObjectPickup;

void ObjectPickupInit(ObjectPickup* object);
void ObjectPickupUpdate(ObjectPickup *object);