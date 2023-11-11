#include "StateMenu.h"

#include <resources.h>
#include <genesis.h>
#include "ui/menu.h"

struct MenuPage* CurrentMenu;
struct Sprite* cursor;

/*
    Forward declare options so they can reference the menu state
*/
void Menu_StartGame();
void Menu_LoadGame();
void Menu_GotoOptions();
void Menu_ReturnToMainMenu();
void Menu_GotoMainMenu();

#define MenuTopSize 3
struct MenuItem MenuTopItems[MenuTopSize] = 
{
    { "START GAME", Menu_StartGame },
    { "LOAD GAME", Menu_LoadGame },
    { "OPTIONS", Menu_GotoOptions },
};
struct MenuPage MenuTop = 
{
    3, MenuTopItems, 0
};

#define MenuOptionsSize 2
struct MenuItem MenuOptionsItems[MenuOptionsSize] =
{
    { "SOUND OPTIONS", Menu_SoundOptions },
    { "BACK TO MAIN MENU", Menu_GotoMainMenu },
};
struct MenuPage MenuOptions = 
{
    2, MenuOptionsItems, 0
};

void Menu_StartGame()
{
    VDP_drawText("StartGame", 0,1);
}
void Menu_LoadGame()
{
    VDP_drawText("LoadGame", 0,1);
}
void Menu_SoundOptions()
{
    VDP_drawText("Sound Options!", 0,1);
}
void Menu_GotoOptions()
{
    CurrentMenu = &MenuOptions;
}
void Menu_GotoMainMenu()
{
    CurrentMenu = &MenuTop;
}

/*
    Menu mechanics
*/
void StateMenu_Joystick(u16 Joy, u16 Changed, u16 State)
{
    if (Changed & State & BUTTON_UP)
    {
        CurrentMenu->Selected = max(CurrentMenu->Selected - 1, 0);
    }
    else if (Changed & State & BUTTON_DOWN)
    {
        CurrentMenu->Selected = min(CurrentMenu->Selected + 1, CurrentMenu->Size - 1);
    }
    else if (Changed & State & (BUTTON_A | BUTTON_B | BUTTON_START))
    {
        CurrentMenu->Items[CurrentMenu->Selected].Callback();
    }
}

void StateMenu_Start()
{
    JOY_setEventHandler(&StateMenu_Joystick);
    Menu_ReturnToMainMenu();
    
    SPR_init();
    cursor = SPR_addSprite(&gfx_cursor, 0, 0, 0);
}
void StateMenu_Tick()
{
    u8 i;
    VDP_drawText("Main Menu",0,0);
    for(i=0; i < CurrentMenu->Size; ++i)
    {
        struct MenuItem* Item = &CurrentMenu->Items[i];
        s8 X = 5;
        s8 Y = 2 + (i * 1);
        VDP_clearText(X, Y, 20);
        VDP_drawText(Item->Label, X + (CurrentMenu->Selected != i),Y);
    }
    u16 SelectedX = 4 * 8;
    u16 SelectedY = (2 + CurrentMenu->Selected) * 8;
    SPR_setPosition(cursor, SelectedX, SelectedY);
    SPR_update();
}