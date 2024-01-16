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
    CT_Zero = 0B0000,
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

// Takes in a generic collision struct, intended argument order is described in function name
bool CheckCollisionPointPoint(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionPointRectangle(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionPointCircle(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionPointLine(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionRectangleRectangle(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionRectangleCircle(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionRectangleLine(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionCircleCircle(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionCircleLine(struct CollisionObject* C1, struct CollisionObject* C2);
bool CheckCollisionLineLine(struct CollisionObject* C1, struct CollisionObject* C2);

bool CheckCollision(struct CollisionObject* Col1, struct CollisionObject* Col2);