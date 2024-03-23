#include "E1M1.h"

#include "../GameContext.h"
#include <genesis.h>

#include <maps.h>
#include <resources.h>

#include "../Types/MapCollision.h"

const int e1m1_tile_xsize = 2 * 8;
const int e1m1_tile_ysize = 2 * 8;

/*
	hint test 
*/
static vs16 WaterLine;            
static vs16 CameraPositionX;
static vs16 CameraPositionY;
static vs16 ForegroundLine;
static vs16 WaterPositionY, WaterPositionX, BackgroundPositionX, BackgroundPositionY;
static vs16 WaterY;

s16 BackgroundScroll[30];

void E1M1VBlank()
{
	WaterY = (308 - 64);	
	WaterLine = (WaterY - CameraPositionY);

	// Reset background scroll
	VDP_setVerticalScroll(BG_B, 0);	//(CameraPositionY >> 2));

	// Where to start foreground overlay
	ForegroundLine = (336 - 32) - CameraPositionY;

	// Reset BG
	VDP_setVerticalScroll(BG_B, 0);
	if(IS_PAL_SYSTEM)
		VDP_setHorizontalScrollTile(BG_B, 0, BackgroundScroll, 30, DMA);
	else
		VDP_setHorizontalScrollTile(BG_B, 0, BackgroundScroll, 28, DMA);

	BackgroundPositionX = (CameraPositionX >> 2);
	BackgroundPositionY = (CameraPositionY >> 3);
	
	SYS_disableInts();

	if(WaterLine > screenHeight || ForegroundLine > screenHeight)		// Waterline off-screen bottom
	{	
		VDP_setHInterrupt(0);	//Disable effect
		PAL_setColors(0, &GameContext.palette[0][0], 64, DMA);	// load normal stage palette
	}
	else if(WaterLine > 1)	// Camera above waterline, on-screen
	{
		VDP_setHInterrupt(1);	// Enable effect
		PAL_setColors(0, &GameContext.palette[0][0], 64, DMA);	// load normal stage palette
	}
	else				// 
	{
		if(ForegroundLine < screenHeight)
			VDP_setHInterrupt(1);
		else
			VDP_setHInterrupt(0);	// Disable int
		PAL_setColors(0, &GameContext.paletteEffect[0][0], 64, DMA);	// Load effect palette
	}
	SYS_enableInts();
}
HINTERRUPT_CALLBACK HIntHandler()
{
	--WaterLine;
	--ForegroundLine;
	if(ForegroundLine == 0)	// BG_B foreground starts here
	{
		VDP_setVerticalScroll(BG_B, 240);
	}
	if(WaterLine == 0)
	{
    	//PAL_setColors(0,&GameContext.paletteEffect[0][0], 64, DMA);
    	//VDP_setVerticalScroll(BG_B, WaterPositionY);
	}
}

// Tile IDs
u16 tile_id;
u16 bg_tile_id;

static const int e1m1_width = 58;
static const int e1m1_height = 21;


const u16 e1m1_collisions[1218] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7, 1, 1, 1, 1, 1, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

u8 E1M1_Collide(const s16 x, const s16 y)
{
    const s16 tx = (x / e1m1_tile_xsize);
    const s16 ty = (y / e1m1_tile_ysize);

	const uint16_t Tile = e1m1_collisions[tx + (ty * e1m1_width)];

	if(Tile == TILE_NONE)
	{
		return 0;
	}
	if(Tile == TILE_BLOCK)
	{
		return true;
	}
	const s16 fracx = x % 16; 
	const s16 fracy = y % 16;

	u16 Hit;

	kprintf("%i : %i", fracx, fracy);

	switch(Tile)
	{
		case TILE_SLOPE_HIGH_LOW_LEFT:
		{
			if(fracy > 8)	
				Hit = TILE_EFFECT_SOLID;
			else
				Hit = ((fracx * 2) > fracy) ? TILE_EFFECT_SOLID : TILE_EFFECT_NONE;
		}
		break;
		case TILE_SLOPE_HIGH_LOW_RIGHT:
			if(fracy < 8)	
				Hit = TILE_EFFECT_NONE;
			else
				Hit =  (((fracx - 8) * 2) > fracy) ? TILE_EFFECT_SOLID : TILE_EFFECT_NONE;
		default:
			return TILE_EFFECT_NONE;
		break;
	}
	
	if(Hit != TILE_EFFECT_NONE)
	{
		MapCollision.TileX = tx;
		MapCollision.TileY = ty;
		MapCollision.PixelX = x;
		MapCollision.PixelY = y;
		return Tile;
	}

    return false;
}

uint16_t E1M1_Init()
{
    GameContext.PlayerSpawn.x = 128;
    GameContext.PlayerSpawn.y = 90;
	WaterY = 512;

	SYS_setHIntCallback(HIntHandler);
	SYS_setVBlankCallback(E1M1VBlank);
	VDP_setHIntCounter(0);
	VDP_setHInterrupt(1);

	VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);

	tile_id = 0;

	VDP_loadTileSet(&bg_e1m1.tileset, tile_id, DMA);
	uint16_t bg_index = tile_id;
	tile_id += bg_e1m1.tileset->numTile;

	uint16_t tile_index = tile_id;
	VDP_loadTileSet(&ts_stage_01, tile_index, DMA);
	tile_id += ts_stage_01.numTile;

	//VDP_drawImage(BG_B, &bg_e1m1, 128,0);
	//VDP_drawImage(BG_B, &bg_e1m1, 256,0);
	//VDP_drawImage(BG_B, &bg_e1m1, 384,0);
    
	GameContext.MapA = MAP_create(&map_stage_01, BG_A, TILE_ATTR_FULL(PAL_TILES, FALSE, FALSE, true, tile_index));
	//GameContext.MapB = MAP_create(&map_stage_01_bg, BG_B, bg_tile_id);

	// Generate temp water palette, please fix
    u16 pal[16];
    for(int i = 0; i < 16; ++i)
	{
        pal[i] = pal_stage_01b.data[i];
	}
	memcpy(&GameContext.paletteEffect[PAL_BACKGROUND],  pal, 16 * 2);
    for(int i = 0; i < 16; ++i)
	{
        pal[i] = pal_stage_01a.data[i];
	}
	memcpy(&GameContext.paletteEffect[PAL_TILES],  pal, 16 * 2);

	// buffer the palettes to global memory, these get sent to hardware on vsync
	memcpy(&GameContext.palette[PAL_BACKGROUND],  bg_e1m1.palette->data, 16 * 2);
	memcpy(&GameContext.palette[PAL_TILES],  pal_stage_01b.data, 16 * 2);

	VDP_drawImageEx(BG_B, &bg_e1m1, TILE_ATTR_FULL(PAL_BACKGROUND, FALSE, FALSE, FALSE, bg_index), 0,0, true, true);
//
	//// Fade In

	return tile_id;
}
void E1M1_Tick()
{
	static int tick = 0;
	++tick;
	const int16_t cloudscroll = -(GameContext.Frame / 5);
	const int16_t wobble1 = -(CameraPositionX / 10);
	const int16_t wobble2 = -(CameraPositionX / 3);
	for(int i = 00; i < 13; ++i)
	{
		BackgroundScroll[i] = cloudscroll;
	}
	for(int i = 13; i < 19; ++i)
	{
		BackgroundScroll[i] = wobble1;
	}
	for(int i = 19; i < 30; ++i)
	{
		BackgroundScroll[i] = wobble2;
	}
}
void E1M1_End()
{
	MAP_release(GameContext.MapA);

	SYS_setHIntCallback(NULL);
	SYS_setVBlankCallback(NULL);
	VDP_setHInterrupt(false);
}
void E1M1_Draw(s16 CameraX, s16 CameraY)
{
	CameraPositionX = CameraX;
	CameraPositionY = CameraY;

    if(GameContext.MapA != NULL)
        MAP_scrollTo(GameContext.MapA, CameraX, CameraY);
	
    //if(GameContext.MapB != NULL)
    //    MAP_scrollTo(GameContext.MapB, (CameraX >> 2), (CameraY >> 2));
}

const StageBase E1M1 = 
{
    "E1M1",
    E1M1_Init,
    E1M1_Tick,
    E1M1_Collide,
    E1M1_Draw,
    E1M1_End,
	928,336,
};