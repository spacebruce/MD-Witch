#include <genesis.h>
#include "defines.h"

#include "GameContext.h"
#include "resources.h"

#include "StateGame.h"
#include "StateMenu.h"

Map *map;

int main(bool hardreset)
{
    if(!hardreset)
        SYS_hardReset();
    JOY_init();
    SPR_init();

	GameContextInit();

	GameContext.CurrentStateID = 0xFF;
	GameContext.NextStateID = STATE_MENU;
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