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
    s16 X1,Y1,X2,Y2,
};

/*
    Make it generic
*/
enum CollisionType
{
    Point = 0B0001,
    Rectangle = 0B0010,
    Circle = 0B0100,
    Line = 0B1000,
};

struct CollisionObject
{
    union 
    {
        CollisionPoint Point;
        CollisionRectangle Rectangle;
        CollisionCircle Circle;
        CollisionLine Line;
    } Collision;
    CollisionType Type;
};

/*
    The functions are built with the utmost confidence they will be used correctly as there is no incoming type checking for speed reasons
    Note the functions have the name scheme and arguments in bit order
        CollisionPoint (0001) < CollisionLine (1000) 
    This is to aid the lookup process later
*/
bool CheckCollisionPointPoint(struct CollisionObject* C1, struct CollisionObject* C2)
{
    return ((C1->Collision.Point.X == C2->Collision.Point.X) && (C1->Collision.Point.Y == C2->Collision.Point.Y));
}
bool CheckCollisionPointRectangle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionPoint* P1 = C1->Collision.Point;
    CollisionRectangle* R2 = C2->Collision.Rectangle;
    return ((P1->X >= R2->X1) && (P1->Y >= R2->Y1)  && (P1->Y <= R2->Y2) && (P1->X <= R2->X2));
};
bool CheckCollisionPointCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionPoint* P1 = C1->Collision.Point;
    CollisionCircle* C2 = C2->Collision.Circle;
}
bool CheckCollisionPointLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionPoint* P1 = C1->Collision.Point;
    CollisionLine* L2 = C2->Collision.Line;
    // bresenhams...?
}
//
bool CheckCollisionRectangleRectangle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionRectangle* R1 = C1->Collision.Rectangle;
    CollisionRectangle* R2 = C2->Collision.Rectangle;
}
bool CheckCollisionRectangleCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionRectangle* R1 = C1->Collision.Rectangle;
    CollisionCircle* S2 = C2->Collision.Circle;
}
bool CheckCollisionRectangleLine(struct CollisionObject* C1, struct CollisionObject C2)
{
    CollisionRectangle* R1 = C1->Collision.Rectangle;
    CollisionLine* L2 = C2->Collision.Line;
}
//
bool CheckCollisionCircleCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionCircle* S1 = C1->Collision.Circle;
    CollisionCircle* S2 = C2->Collision.Circle;
}
bool CheckCollisionCircleLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionCircle* S1 = C1->Collision.Circle;
    CollisionLine* L2 = C2->Collision.Line;
}
//
bool CheckCollisionLineLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    CollisionLine* S1 = C1->Collision.Line;
    CollisionLine* S2 = C2->Collision.Line;
}

/*
    Burn some ROM in exchange for a lookup table
    only 1 or 2 active bit combinations allowed, 3 or more is a fail
*/
typedef bool (*CollisionLookupFunction)(void* C1, void* C2);

bool (*CollisionLookup[16]) (void* C1, void* C2) = 
{
    nullptr,                                // 0000 nope
    CheckCollisionPointPoint,               // 0001
    CheckCollisionRectangleRectangle,       // 0010
    CheckCollisionPointRectangle,           // 0011
    CheckCollisionCircleCircle,             // 0100
    CheckCollisionPointCircle,              // 0101
    CheckCollisionRectangleCircle,          // 0110
    nullptr,                                // 0111 nope
    CheckCollisionLineLine,                 // 1000
    CheckCollisionPointLine,                // 1001
    CheckCollisionRectangleLine,            // 1010 
    nullptr,                                // 1011 nope
    CheckCollisionCircleLine,               // 1100
    nullptr,                                // 1101 nope
    nullptr,                                // 1110 nope
    nullptr,                                // 1111 nope
};

bool CheckCollision(struct CollisionObject* Col1, struct CollisionObject* Col2)
{
    // Native 16-bit integers so should save a mask op hopefully
    const uint16_t MaskLeft = Col1->Type;               // 0010 0100 <- left type
    const uint16_t MaskRight = Col2->Type;              // 0001 0100 <- right type
    const uint16_t MaskIndex = (MaskLeft | MaskRight);  // 0011 0100 <- combined makes a lookup index

    // grab ptr to collision function
    CollisionLookupFunction Call = CollisionLookup[MaskIndex];  

    // Invalid combination, bail
    if(Call == nullptr) 
        return false;
    
    // Use bit order to figure which order collision objects go in
    if(MaskLeft <= MaskRight)
        return Call(Col1->Collision, Col2->Collision);
    else
        return Call(Col2->Collision, Col1->Collision);
}
