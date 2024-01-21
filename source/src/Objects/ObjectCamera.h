#pragma once

#include <genesis.h>
#include "ObjectBase.h"

typedef struct ObjectCamera
{
    struct ObjectBase Base;
    struct ObjectBase* Target;  // Ptr to target
    u16 StageWidth;
    u16 StageHeight;
} ObjectCamera;

void ObjectCameraSetStageSize(ObjectCamera* Camera, const u16 W, const u16 H);
void ObjectCameraSetTarget(ObjectCamera* Camera, struct ObjectBase* Target);
void ObjectCameraInit(ObjectCamera* Camera, struct ObjectBase* Target);
void ObjectCameraUpdate(ObjectCamera* Camera);
void ObjectCameraFreecam(ObjectCamera* Camera, u16 State);