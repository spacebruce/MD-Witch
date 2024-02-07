#include "Stages.h"

const StageBase* GetStageData(u8 stageID)
{
    for (u8 i = 0; i < sizeof(Stages) / sizeof(Stages[0]); ++i)
    {
        if (Stages[i].ID == stageID)
        {
            return Stages[i].Pointer;
        }
    }
    return NULL;    // Return NULL if the stage ID is not found
}