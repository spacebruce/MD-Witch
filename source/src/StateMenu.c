#include "StateMenu.h"

#include <resources.h>
#include <genesis.h>
#include "Types/Menu.h"

#include "GameContext.h"
#include "defines.h"

struct MenuPage* CurrentMenu;

struct Sprite* cursor[3];

fix16 SCROLL_Y = FIX16(0);

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
    GameContext.NextStateID = STATE_GAME;
    PAL_fadeOutAll((u16)GameContext.Framerate, false);
    VDP_drawText("StartGame", 0,36);
}

void Menu_LoadGame()
{
    VDP_drawText("LoadGame", 0,36);
}
void Menu_SoundOptions()
{
    VDP_drawText("Sound Options!", 0,36);
}
void Menu_VideoOptions()
{
    VDP_drawText("Video Options!", 0,36);
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
    SCROLL_Y = FIX16(0);

    JOY_setEventHandler(&StateMenu_Joystick);
    Menu_GotoMainMenu();    // Init menu
    
	VDP_setPlaneSize(64,64, true);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setHorizontalScroll(BG_B, 0);
    VDP_setVerticalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_B, 0);

	VDP_drawImage(BG_A, &sprLogo,18,29);
	DMA_waitCompletion();
	VDP_drawImage(BG_B, &sprWitch,1,32);
	DMA_waitCompletion();

    //PAL_fadeIn(0,63, image_titlescreen.palette->data, GameContext.Framerate * 2, true);

    VDP_setTextPlane(BG_A);

    SPR_init();
    for(int i = 0; i < 3; ++i)
    {
        cursor[i] = SPR_addSprite(&gfx_cursor, 0, 0, TILE_ATTR(PAL_STUFF, 1, false, false));
        //Sprite* spr = cursor[i];
        SPR_setPalette(cursor[i], PAL_STUFF);
    }
    //PAL_setPalette(STUFF_PALETTE, gfx_cursor.palette->data, DMA);
}
void StateMenu_End()
{
    //PAL_fadeOutAll(GameContext.Framerate, false);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_clearSprites();
    SPR_end();
}

char buffer[32];
void StateMenu_Tick()
{
    int ScrollInt = fix16ToInt(SCROLL_Y);
    VDP_setVerticalScroll(BG_B, ScrollInt / 2);    
    VDP_setVerticalScroll(BG_A, min(ScrollInt, 216));    
    if((ScrollInt / 2) <= 250)
    {
        SCROLL_Y = fix16Add(SCROLL_Y, FIX16(1.2));
    }
    sprintf(buffer, "%i", ScrollInt);
    VDP_drawText(buffer,5,38);

    u8 i;
    s16 X, Y, CX,CY;
    Y = 50 + (CurrentMenu != &MenuTop); //Offset 1 tile down if a submenu
    VDP_clearText(0, Y, 40);
    VDP_clearText(0, Y + 1, 40);
    for(i=0; i < CurrentMenu->Size; ++i)
    {
        struct MenuItem* Item = &CurrentMenu->Items[i];
        Y = 47 + (i * 3);
        CX = 24;
        SPR_setAnim(cursor[i], (CurrentMenu->Selected == i));

        SPR_setPosition(cursor[i],CX * 8,Y);
        VDP_drawText(Item->Label, CX + 2,Y);
    }
    
    char mode[5];
    if(IS_PAL_SYSTEM)
    {
        strncpy(mode, "PAL\0",4);
    } else
    {
        strncpy(mode,"NTSC\0",5);
    }
    sprintf(buffer, "VIDEO : %s (%ihz)", mode, GameContext.Framerate);
    VDP_drawText(buffer,2,54);

    SPR_update();
}
void StateMenu_EndTick()
{
}

const StateType StateMenu = 
{
    StateMenu_Start, StateMenu_End, StateMenu_Tick, 
};