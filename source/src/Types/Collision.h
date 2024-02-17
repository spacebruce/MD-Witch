#pragma once
#include <genesis.h>

typedef enum
{
    TopLeft, TopMiddle, TopRight,
    MiddleLeft, Middle, MiddleRight,
    BottomLeft, BottomMiddle, BottomRight,
} CT_ALIGN;

/*
    Data for every type of collision obj
*/
struct CollisionPoint
{
    s16 X,Y;
};
struct CollisionRectangle
{
    s16 X1,Y1, X2,Y2;
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
    Store data alongside collision
*/
typedef enum 
{
    NO_DATA  = 0,
    DAMAGE_PLAYER,
    DAMAGE_ENEMY,
    HEAL_PLAYER,
    TELEPORTER,
    LEVEL_END,
} Collision_Datatype;

struct CollisionData
{
    Collision_Datatype Type : 8;
    u16 Data : 16;
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
    CollisionType Type : 8;
    struct CollisionData Data;
    struct CollisionObject* Next;
    struct CollisionObject* Prev;
};

// Metadata
void ClearCollisionData(struct CollisionObject* Object);
void SetCollisionData(struct CollisionObject* Object, const CollisionType Type, const u16 Data);

// Move shapes
void SetCollisionRectangle(struct CollisionObject* Object, const s16 X, const s16 Y, const s16 Width, const  s16 Height);
void SetCollisionRectangleAligned(struct CollisionObject* Object, const s16 X, const s16 Y, const s16 Width, const s16 Height, CT_ALIGN Align);
void SetCollisionCircle(struct CollisionObject* Object, const s16 X, const s16 Y, const s16 Radius);

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