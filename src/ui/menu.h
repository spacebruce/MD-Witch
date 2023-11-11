#pragma once
#include <genesis.h>

typedef void (*MenuCallback)();

struct MenuItem
{
    const char* Label;
    MenuCallback Callback;
};
struct MenuPage
{
    u8 Size;
    struct MenuItem* Items;
    u8 Selected;
};