#pragma once

#include "../ObjectBase.h"

typedef struct EnemyBloober
{
    struct ObjectBase Base;
} EnemyBloober;

void EnemyBlooberInit(void* object);
void EnemyBlooberUpdate(void* object);
void EnemyBlooberFree(void* object);