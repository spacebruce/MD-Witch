#pragma once
#include <genesis.h>

#include "Objects/ObjectBase.h"

typedef void (ObjectFunctionVoid)(void*);

typedef enum 
{
    TypeObjectCamera = 0,
    TypeObjectPlayer = 1,
    TypeObjectPickup = 2,
//
    TypeEnemyBloober = 100,
    TypeEnemyFrogman = 101,
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
#include "Objects/Enemies/EnemyBloober.h"
#include "Objects/Enemies/EnemyFrogman.h"

static const struct ObjectManifest ObjectList[] = 
{
    // Important stuff
    { TypeObjectCamera, "CAMERA", sizeof(ObjectCamera), ObjectCameraInit, ObjectCameraUpdate, NULL },
    { TypeObjectPlayer, "PLAYER", sizeof(ObjectPlayer), ObjectPlayerInit, ObjectPlayerUpdate, NULL },
    { TypeObjectPickup, "PICKUP", sizeof(ObjectPickup), ObjectPickupInit, ObjectPickupUpdate, ObjectPickupKill },
    // 
    
    //  Enemies
    { TypeEnemyBloober, "BLOOBER", sizeof(EnemyBloober), EnemyBlooberInit, EnemyBlooberUpdate, EnemyBlooberFree },
    { TypeEnemyFrogman, "FROGMAN", sizeof(NULL), NULL, NULL, NULL },
};