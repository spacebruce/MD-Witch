#pragma once
#include <genesis.h>

#include "Objects/ObjectBase.h"

typedef void (ObjectFunctionVoid)(void*);

typedef enum 
{
    TypeObjectCamera = 0,
    TypeObjectPlayer = 1,
    TypeObjectPickup = 2,
} ObjectID;

struct ObjectManifest
{
    ObjectID ID;
    char name[8];
    uint16_t Size;
    ObjectFunctionVoid* Init;
    ObjectFunctionVoid* Tick;
    ObjectFunctionVoid* Kill;
};

#include "Objects/ObjectCamera.h"
#include "Objects/ObjectPlayer.h"
#include "Objects/ObjectPickup.h"

static const struct ObjectManifest ObjectList[] = 
{
    { TypeObjectCamera, "CAMERA", sizeof(ObjectCamera), ObjectCameraInit, ObjectCameraUpdate, NULL },
    { TypeObjectPlayer, "PLAYER", sizeof(ObjectPlayer), ObjectPlayerInit, ObjectPlayerUpdate, NULL },
    { TypeObjectPickup, "PICKUP", sizeof(ObjectPickup), ObjectPickupInit, ObjectPickupUpdate, NULL },
};