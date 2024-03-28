#pragma once
#include <genesis.h>

#include "../Types/StageBase.h"

struct StageGroup
{
    const u8 ID;
    const StageBase* Pointer;
};

#include "E1M0.h"
#include "E1M1.h"
//#include "E1M2.h"
//#include "E1M3.h"

// da level lookup table
static const struct StageGroup Stages[] = 
{
    { 0, &E1M0 },
    { 1, &E1M1 },
    // { 2, &E1M2 },
    // { 3, &E1M3 },
};

// Transform stage ID to pointer
const StageBase* GetStageData(u8 stageID);