#pragma once
#include "ObjectBase.h"

struct ObjectPlayer
{
    struct ObjectBase Base;
};

void updateObjectPlayer(struct ObjectBase *object)
{
    struct ObjectPlayer *ObjectPlayer = (struct ObjectPlayer *)object;
}