#pragma once

/*
    Improvements sorely needed
    [ ] Ditch malloc/free for custom solution
    [ ] Bind functions to objects rather than use the (slower?) look-up table method
    [ ] Switch to linked list instead of array-backed
*/

#include "ObjectList.h"
#include <maths.h>

void InitObjectManager();
void EndObjectManager();
void* CreateObject(const ObjectID Type, fix32 X, fix32 Y);
void DestroyObject(const uint8_t ID);
void TickObjects();

void* GetObjectFromID(const uint8_t ID);