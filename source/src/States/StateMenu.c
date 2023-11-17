#include "StateMenu.h"

#include <resources.h>
#include <genesis.h>
#include "ui/menu.h"

#include "State.h"

struct MenuPage* CurrentMenu;
struct Sprite* cursor;

// Forward declare options here so they can poke state around later
void Menu_StartGame();
void Menu_LoadGame();
void Menu_GotoOptions();
void Menu_SoundOptions();
void Menu_VideoOptions();
void Menu_GotoMainMenu();

// Define menu constructs here
#define MenuTopSize 3
struct MenuItem MenuTopItems[MenuTopSize] = 
{
    { "START", Menu_StartGame },
    { "LOAD", Menu_LoadGame },
    { "OPTIONS", Menu_GotoOptions },
};
struct MenuPage MenuTop = 
{
    MenuTopSize, MenuTopItems, 0
};

#define MenuOptionsSize 3
struct MenuItem MenuOptionsItems[MenuOptionsSize] =
{
    { "SOUND", Menu_SoundOptions },         // "label", (void*)()
    { "VIDEO", Menu_VideoOptions },         // "label", (void*)()
    { "BACK^", Menu_GotoMainMenu },
};
struct MenuPage MenuOptions = 
{
    MenuOptionsSize, MenuOptionsItems, 0
};

// Now implement the callbacks here
void Menu_StartGame()
{
    VDP_drawText("StartGame", 0,1);
    STATE_NEXT = STATE_GAME;
}
void Menu_LoadGame()
{
    VDP_drawText("LoadGame", 0,1);
}
void Menu_SoundOptions()
{
    VDP_drawText("Sound Options!", 0,1);
}
void Menu_VideoOptions()
{
    VDP_drawText("Video Options!", 0,1);
}
void Menu_GotoOptions()
{
    CurrentMenu = &MenuOptions;
}
void Menu_GotoMainMenu()
{
    CurrentMenu = &MenuTop;
}

// Joystick
void StateMenu_Joystick(u16 Joy, u16 Changed, u16 State)
{
    if (Changed & State & BUTTON_LEFT)
    {
        CurrentMenu->Selected = max(CurrentMenu->Selected - 1, 0);
    }
    else if (Changed & State & BUTTON_RIGHT)
    {
        CurrentMenu->Selected = min(CurrentMenu->Selected + 1, CurrentMenu->Size - 1);
    }
    else if (Changed & State & (BUTTON_A | BUTTON_B | BUTTON_START))
    {
        CurrentMenu->Items[CurrentMenu->Selected].Callback();
    }
}

// State entry points
void StateMenu_Start()
{
    JOY_setEventHandler(&StateMenu_Joystick);
    Menu_GotoMainMenu();    // Init menu
    
    SPR_init();
    cursor = SPR_addSprite(&gfx_cursor, 0, 0, 0);
}
void StateMenu_End()
{
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_clearSprites();
    SPR_end();
}
void StateMenu_Tick()
{
    VDP_drawText("Main Menu",0,0);
    u8 i;
    s16 X, Y, CX,CY;
    Y = 22 + (CurrentMenu != &MenuTop); //Offset 1 tile down if a submenu
    VDP_clearText(0, Y, 40);
    VDP_clearText(0, Y + 1, 40);
    for(i=0; i < CurrentMenu->Size; ++i)
    {
        struct MenuItem* Item = &CurrentMenu->Items[i];
        X = 6 + (i * 10);
        if(CurrentMenu->Selected == i)
        {
            CX = (X - 1) * 8;
            CY = Y * 8;
        }
        VDP_drawText(Item->Label, X,Y);
    }
    SPR_setPosition(cursor, CX, CY);
    SPR_update();
}
void StateMenu_EndTick()
{
}

struct StateType StateMenu = 
{
    StateMenu_Start, StateMenu_End, StateMenu_Tick, StateMenu_EndTick,
};