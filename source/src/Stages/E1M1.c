    #include "E1M1.h"

    #include "../GameContext.h"
    #include <genesis.h>
    #include <resources.h>

    uint8_t e1m1_collisions[8][8] = 
    {
        { 0, 0, 0, 1, 0, 0, 0, 1, },
        { 0, 0, 0, 0, 0, 0, 0, 0, },
        { 0, 0, 0, 0, 0, 0, 0, 0, },
        { 0, 0, 0, 0, 0, 0, 0, 0, },
        { 0, 0, 0, 0, 0, 1, 1, 1, },
        { 0, 0, 0, 0, 1, 0, 0, 0, },
        { 0, 0, 0, 1, 0, 0, 0, 0, },
        { 1, 1, 1, 0, 0, 0, 0, 0, },
    };

    void E1M1_Init()
    {
        GameContext.PlayerSpawn.x = 0;
        GameContext.PlayerSpawn.y = 50;

        for(int x = 0; x < 8; ++x)
        {
            for(int y = 0; y < 8; ++y)
            {
                if(e1m1_collisions[y][x] == 1)
                {
                    VDP_drawText("X",x * 6, y * 4);
                }
            }
        }
        DMA_waitCompletion();

        VDP_setTextPlane(BG_B);

        for(int i = 0; i < 32; ++i)
            VDP_drawText(GameContext.CurrentStage->Name,0,i);
            

    }
    void E1M1_Tick()
    {
    }
    void E1M1_End()
    {
    }
    void E1M1_Draw()
    {
    }

    const StageBase E1M1 = 
    {
        "E1M1",
        E1M1_Init,
        E1M1_Tick,
        E1M1_Draw,
        E1M1_End,
    };