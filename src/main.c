#include <genesis.h>
#include "game.h"


#include "StateMenu.h"

int main()
{
	StateMenu_Start();
	while(1)
	{        
		StateMenu_Tick();
		SYS_doVBlankProcess();
	}
	return (0);
}