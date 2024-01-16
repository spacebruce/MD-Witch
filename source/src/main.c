#include <genesis.h>
#include "defines.h"

#include "GameContext.h"
#include "resources.h"

#include "StateGame.h"
#include "StateMenu.h"

int main(bool hardreset)
{
	// Game engine stuff
	if(!hardreset)
	{
		SYS_hardReset();
	}

	SYS_disableInts();

    JOY_init();
    SPR_init();

	// Settings
	GameContextInit();

	// Video
    VDP_setScreenWidth320();
	if(IS_PAL_SYSTEM)
	{
		VDP_setScreenHeight240();
	}
	else
	{
		VDP_setScreenHeight224();
	}

	// Boot directly into the menu
	GameContext.CurrentStateID = 0xFF;
	GameContext.NextStateID = STATE_MENU;

	SYS_enableInts();
	while(1)
	{
		if(GameContext.CurrentStateID != GameContext.NextStateID)
		{
			if(GameContext.CurrentStateID != 0xFF)
			{
				GameContext.CurrentState->End();
			}
			GameContext.CurrentStateID = GameContext.NextStateID;
			switch(GameContext.CurrentStateID)
			{
				case STATE_MENU:	GameContext.CurrentState = &StateMenu;	break;
				case STATE_GAME:	GameContext.CurrentState = &StateGame;	break;
			}
			GameContext.CurrentState->Start();
		}
		GameContext.CurrentState->Tick();
		GameContextTick();
    	SYS_doVBlankProcess();
	}
	return (0);
}