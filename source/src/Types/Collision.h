#pragma once
#include <genesis.h>

/*
    Data for every type of collision obj
*/
struct CollisionPoint
{
    s16 X,Y;
};
struct CollisionRectangle
{
    s16 X1,Y1,X2,Y2;
};
struct CollisionCircle
{
    s16 X,Y,Radius;
};
struct CollisionLine
{
    s16 X1,Y1,X2,Y2;
};

/*
    Make it generic
*/
typedef enum
{
    CT_Point = 0B0001,
    CT_Rectangle = 0B0010,
    CT_Circle = 0B0100,
    CT_Line = 0B1000,
} CollisionType;

struct CollisionObject
{
    union 
    {
        struct CollisionPoint Point;
        struct CollisionRectangle Rectangle;
        struct CollisionCircle Circle;
        struct CollisionLine Line;
    } Collision;
    CollisionType Type;
};

/*
    The functions are built with the utmost confidence they will be used correctly as there is no incoming type checking for speed reasons
    Note the functions have the name scheme and arguments in bit order
        CollisionPoint (0001) < CollisionLine (1000) 
    This is to aid the lookup process later
*/