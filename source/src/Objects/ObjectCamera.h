#pragma once

#include <genesis.h>
#include "ObjectBase.h"

typedef struct ObjectCamera
{
    struct ObjectBase Base;
    struct ObjectBase* Target;  // Ptr to target
} ObjectCamera;


void ObjectCameraSetTarget(ObjectCamera* Camera, struct ObjectBase* Target);
void ObjectCameraInit(ObjectCamera* Camera, struct ObjectBase* Target);
void ObjectCameraUpdate(ObjectCamera* Camera);
void ObjectCameraFreecam(ObjectCamera* Camera, u16 Changed, u16 State);