#pragma once
#include <genesis.h>

struct ObjectBase
{
    fix32 Position;
    void (*update)(struct ObjectBase *);
};

