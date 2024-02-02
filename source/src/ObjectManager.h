#pragma once

/*

*/

#include "ObjectList.h"

void InitObjectManager();
void EndObjectManager();
void* CreateObject(const ObjectID Type);
void DestroyObject(const uint8_t ID);
void TickObjects();

void* GetObjectFromID(const uint8_t ID);