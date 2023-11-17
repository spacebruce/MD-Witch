#pragma once
#include <genesis.h>
#include "StageBase.h"

typedef struct
{
    const u8 ID;
    const StageBase* Pointer;
} StageGroup;

#include "E1M1.h"

/*
    Map of stage ID's to the correct data structure
*/
static const StageGroup Stages[] = 
{
    // { 0, &Cutscene1 },
    { 1, &E1M1 },
    // { 2, &Stage12 },
    // { 3, &Stage13 },
};

inline static const StageBase* GetLevelFromID(const u8 ID)
{
    for(uint8_t i = 0; i < sizeof(Stages); ++i)
    {
        if(Stages[i].ID == ID)
            return Stages[i].Pointer;
    }
    return NULL;
}