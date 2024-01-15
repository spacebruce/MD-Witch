#include "Collision.h"


bool CheckCollisionPointPoint(struct CollisionObject* C1, struct CollisionObject* C2)
{
    return ((C1->Collision.Point.X == C2->Collision.Point.X) && (C1->Collision.Point.Y == C2->Collision.Point.Y));
}
bool CheckCollisionPointRectangle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionPoint* P1 = &C1->Collision.Point;
    struct CollisionRectangle* R2 = &C2->Collision.Rectangle;
    return ((P1->X >= R2->X1) && (P1->Y >= R2->Y1)  && (P1->Y <= R2->Y2) && (P1->X <= R2->X2));
};
bool CheckCollisionPointCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionPoint* P1 = &C1->Collision.Point;
    struct CollisionCircle* S2 = &C2->Collision.Circle;
    return FALSE;
}
bool CheckCollisionPointLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionPoint* P1 = &C1->Collision.Point;
    struct CollisionLine* L2 = &C2->Collision.Line;
    // bresenhams...?
    return FALSE;
}
//
bool CheckCollisionRectangleRectangle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionRectangle* R1 = &C1->Collision.Rectangle;
    struct CollisionRectangle* R2 = &C2->Collision.Rectangle;
    return FALSE;
}
bool CheckCollisionRectangleCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionRectangle* R1 = &C1->Collision.Rectangle;
    struct CollisionCircle* S2 = &C2->Collision.Circle;
    return FALSE;
}
bool CheckCollisionRectangleLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionRectangle* R1 = &C1->Collision.Rectangle;
    struct CollisionLine* L2 = &C2->Collision.Line;
    return FALSE;
}
//
bool CheckCollisionCircleCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionCircle* S1 = &C1->Collision.Circle;
    struct CollisionCircle* S2 = &C2->Collision.Circle;
    return FALSE;
}
bool CheckCollisionCircleLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionCircle* S1 = &C1->Collision.Circle;
    struct CollisionLine* L2 = &C2->Collision.Line;
    return FALSE;
}
//
bool CheckCollisionLineLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionLine* S1 = &C1->Collision.Line;
    struct CollisionLine* S2 = &C2->Collision.Line;
    return FALSE;
}

/*
    Burn some ROM in exchange for a lookup table
    only 1 or 2 active bit combinations allowed, 3 or more is a fail
*/

typedef bool (*CollisionLookupFunction)(void* C1, void* C2);

bool (*CollisionLookup[16]) (void* C1, void* C2) = 
{
    NULL,                                   // 0000 nope
    CheckCollisionPointPoint,               // 0001
    CheckCollisionRectangleRectangle,       // 0010
    CheckCollisionPointRectangle,           // 0011
    CheckCollisionCircleCircle,             // 0100
    CheckCollisionPointCircle,              // 0101
    CheckCollisionRectangleCircle,          // 0110
    NULL,                                // 0111 nope
    CheckCollisionLineLine,                 // 1000
    CheckCollisionPointLine,                // 1001
    CheckCollisionRectangleLine,            // 1010 
    NULL,                                // 1011 nope
    CheckCollisionCircleLine,               // 1100
    NULL,                                // 1101 nope
    NULL,                                // 1110 nope
    NULL,                                // 1111 nope
};


/*

*/

bool CheckCollision(struct CollisionObject* Col1, struct CollisionObject* Col2)
{
    // Native 16-bit integers so should save a mask op hopefully
    const uint16_t MaskLeft = Col1->Type;               // 0010 0100 <- left type
    const uint16_t MaskRight = Col2->Type;              // 0001 0100 <- right type
    const uint16_t MaskIndex = (MaskLeft | MaskRight);  // 0011 0100 <- combined makes a lookup index

    // grab ptr to collision function
    CollisionLookupFunction Call = CollisionLookup[MaskIndex];  

    // Invalid combination, bail
    if(Call == NULL) 
        return FALSE;
    
    // Use bit order to figure which order collision objects go in
    if(MaskLeft <= MaskRight)
        return Call(Col1, Col2);
    else
        return Call(Col2, Col1);
}
