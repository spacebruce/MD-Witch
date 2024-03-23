#pragma once

enum TILE_TYPE
{
	TILE_NONE = 0,
	TILE_BLOCK =  1,
	TILE_RESERVED1 = 2,
	TILE_RESERVED2 = 3,
	TILE_SLOPE_HIGH_LOW_LEFT = 4,
	TILE_SLOPE_HIGH_LOW_RIGHT = 5,
	TILE_SLOPE_LOW_HIGH_LEFT = 6,
	TILE_SLOPE_LOW_HIGH_RIGHT = 7
};

enum TILE_EFFECT
{
    TILE_EFFECT_NONE = 0, 
    TILE_EFFECT_SOLID = 1,
};

typedef struct CollisionReturn
{
    u16 TileX;
    u16 TileY;
    s16 PixelX;
    s16 PixelY;
} CollisionReturn;

CollisionReturn MapCollision;