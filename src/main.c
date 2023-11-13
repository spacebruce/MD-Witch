#include <genesis.h>

#include "States/State.h"
#include "States/StateMenu.h"
#include "States/StateGame.h"

int main(bool hardreset)
{
	// Init console
	if(!hardreset)
		SYS_hardReset();
	JOY_init();
	SPR_init();

	// Messy state machine, gomenasai
	while(1)
	{        
		if(STATE_CURRENT != STATE_NEXT)
		{
			if(STATE_CURRENT != 0xFF)
				StatePtr->End();
			STATE_CURRENT = STATE_NEXT;
			switch(STATE_CURRENT)
			{
				case STATE_MENU:	StatePtr = &StateMenu;	break;
				case STATE_GAME:	StatePtr = &StateGame;	break;
			}
			StatePtr->Start();
		}
		else
		{
			StatePtr->Tick();
		}
		SYS_doVBlankProcess();
	}
	return (0);
}