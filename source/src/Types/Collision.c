#include "Collision.h"

#include "NumericHelpers.h"

void SetCollisionRectangle(struct CollisionObject* Object, const s16 X, const s16 Y, const s16 Width, const s16 Height)
{
    Object->Type = CT_Rectangle;
    Object->Collision.Rectangle.X1 = X - (Width / 2);
    Object->Collision.Rectangle.X2 = X + (Width / 2);
    Object->Collision.Rectangle.Y1 = Y - (Height / 2);
    Object->Collision.Rectangle.Y2 = Y + (Height / 2);
}
void SetCollisionRectangleAligned(struct CollisionObject* Object, const s16 X, const s16 Y, const s16 Width, const s16 Height, CT_ALIGN Align)
{
    Object->Type = CT_Rectangle;
    switch(Align)
    {
        case TopLeft:
            Object->Collision.Rectangle.X1 = X;
            Object->Collision.Rectangle.X2 = X + Width;
            Object->Collision.Rectangle.Y1 = Y;
            Object->Collision.Rectangle.Y2 = Y + Height;
        break;
        case TopMiddle:
            Object->Collision.Rectangle.X1 = X - (Width / 2);
            Object->Collision.Rectangle.X2 = X + (Width / 2);
            Object->Collision.Rectangle.Y1 = Y;
            Object->Collision.Rectangle.Y2 = Y + Height;
        break;
        case TopRight:
            Object->Collision.Rectangle.X1 = X - Width;
            Object->Collision.Rectangle.X2 = X;
            Object->Collision.Rectangle.Y1 = Y;
            Object->Collision.Rectangle.Y2 = Y + Height;
        break;
        case MiddleLeft:
            Object->Collision.Rectangle.X1 = X;
            Object->Collision.Rectangle.X2 = X + Width;
            Object->Collision.Rectangle.Y1 = Y - (Height / 2);
            Object->Collision.Rectangle.Y2 = Y + (Height / 2);
        break;
        case Middle:
            Object->Collision.Rectangle.X1 = X - (Width / 2);
            Object->Collision.Rectangle.X2 = X + (Width / 2);
            Object->Collision.Rectangle.Y1 = Y - (Height / 2);
            Object->Collision.Rectangle.Y2 = Y + (Height / 2);
        break;
        case MiddleRight:
            Object->Collision.Rectangle.X1 = X - Width;
            Object->Collision.Rectangle.X2 = X;
            Object->Collision.Rectangle.Y1 = Y - (Height / 2);
            Object->Collision.Rectangle.Y2 = Y + (Height / 2);
        break;
        case BottomLeft:
            Object->Collision.Rectangle.X1 = X;
            Object->Collision.Rectangle.X2 = X + Width;
            Object->Collision.Rectangle.Y1 = Y - Height;
            Object->Collision.Rectangle.Y2 = Y;
        break;
        case BottomMiddle:
            Object->Collision.Rectangle.X1 = X - (Width / 2);
            Object->Collision.Rectangle.X2 = X + (Width / 2);
            Object->Collision.Rectangle.Y1 = Y - Height;
            Object->Collision.Rectangle.Y2 = Y;
        break;
        case BottomRight:
            Object->Collision.Rectangle.X1 = X - Width;
            Object->Collision.Rectangle.X2 = X;
            Object->Collision.Rectangle.Y1 = Y - Height;
            Object->Collision.Rectangle.Y2 = Y;
        break;
    }
}

void SetCollisionCircle(struct CollisionObject* Object, const s16 X, const s16 Y, const s16 Radius)
{
    Object->Type = CT_Circle;
    Object->Collision.Circle.X = X;
    Object->Collision.Circle.Y = Y;
    Object->Collision.Circle.Radius = Radius;
}

void SetCollisionLine(struct CollisionObject* Object, const s16 X1, const s16 Y1, const s16 X2, const s16 Y2)
{
    Object->Type = CT_Line;
    Object->Collision.Line.X1 = X1;
    Object->Collision.Line.Y1 = Y1;
    Object->Collision.Line.X2 = X2;
    Object->Collision.Line.Y2 = Y2;
}

/*
    These functions are built with the utmost confidence they will be used correctly as there is no incoming type checking for speed reasons
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
    struct CollisionPoint* P1 = &C1->Collision.Point;
    struct CollisionRectangle* R2 = &C2->Collision.Rectangle;
    return ((P1->X >= R2->X1) && (P1->Y >= R2->Y1)  && (P1->Y <= R2->Y2) && (P1->X <= R2->X2));
};
bool CheckCollisionPointCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionPoint* P1 = &C1->Collision.Point;
    struct CollisionCircle* S2 = &C2->Collision.Circle;

    // Basic distance check for circle-circle collision
    s16 distanceSquared = (P1->X - S2->X) * (P1->X - S2->X) + (P1->Y - S2->Y) * (P1->Y - S2->Y);
    s16 radiusSumSquared = (S2->Radius) * (S2->Radius);

    return distanceSquared <= radiusSumSquared;
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

    const bool l1 = ((R1->X1) <= (R2->X2));
    const bool l2 = ((R1->X2) >= (R2->X1));
    const bool l3 = ((R1->Y1) <= (R2->Y2));
    const bool l4 = ((R1->Y2) >= (R2->Y1));

    return (l1 && l2 && l3 && l4);
}

bool CheckCollisionRectangleCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionRectangle* R1 = &C1->Collision.Rectangle;
    struct CollisionCircle* S2 = &C2->Collision.Circle;

    s16 circleDistanceX = abs(S2->X - R1->X1);
    s16 circleDistanceY = abs(S2->Y - R1->Y1);

    if (circleDistanceX > (R1->X2 - R1->X1)/2 + S2->Radius) { return FALSE; }
    if (circleDistanceY > (R1->Y2 - R1->Y1)/2 + S2->Radius) { return FALSE; }

    if (circleDistanceX <= (R1->X2 - R1->X1)/2) { return TRUE; } 
    if (circleDistanceY <= (R1->Y2 - R1->Y1)/2) { return TRUE; }

    s16 cornerDistance_sq = powS16((circleDistanceX - (R1->X2 - R1->X1)/2),2) + powS16((circleDistanceY - (R1->Y2 - R1->Y1)/2),2);

    return (cornerDistance_sq <= powS16(S2->Radius,2));
}
bool CheckCollisionRectangleLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionRectangle* R1 = &C1->Collision.Rectangle;
    struct CollisionLine* L2 = &C2->Collision.Line;

    return (L2->X1 >= R1->X1 && L2->X1 <= R1->X2 && L2->Y1 >= R1->Y1 && L2->Y1 <= R1->Y2) ||
           (L2->X2 >= R1->X1 && L2->X2 <= R1->X2 && L2->Y2 >= R1->Y1 && L2->Y2 <= R1->Y2);
}
bool CheckCollisionCircleCircle(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionCircle* S1 = &C1->Collision.Circle;
    struct CollisionCircle* S2 = &C2->Collision.Circle;

    s16 distX = S1->X - S2->X;
    s16 distY = S1->Y - S2->Y;
    s16 distance = sqrtS16(distX*distX + distY*distY);

    return distance <= (S1->Radius + S2->Radius);
}

bool CheckCollisionCircleLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionLine* L = &C1->Collision.Line;
    struct CollisionCircle* S = &C2->Collision.Circle;

    int x1 = L->X1, y1 = L->Y1, x2 = L->X2, y2 = L->Y2;
    int cx = S->X, cy = S->Y, radius = S->Radius;

    // Calculate the direction vector of the line
    int dx = x2 - x1, dy = y2 - y1;

    // Calculate the vector from the line start to the sphere center
    int x_diff = cx - x1, y_diff = cy - y1;

    // Calculate the projection of x_diff, y_diff onto the line direction vector
    float t = ((float)x_diff * dx + (float)y_diff * dy) / (dx * dx + dy * dy);

    // Find the closest point on the line to the sphere center
    int closest_x = x1 + t * dx;
    int closest_y = y1 + t * dy;

    // Check if the closest point is within the line segment
    if (t >= 0 && t <= 1)
    {
        // Check if the closest point is within the sphere
        //if (sqrt((closest_x - cx) * (closest_x - cx) + (closest_y - cy) * (closest_y - cy)) <= radius)
        if (((closest_x - cx) * (closest_x - cx) + (closest_y - cy) * (closest_y - cy)) <= (radius^radius))
        {
            return TRUE; // Line intersects sphere
        }
    }

    return FALSE; // Line does not intersect sphere
}
//
bool CheckCollisionLineLine(struct CollisionObject* C1, struct CollisionObject* C2)
{
    struct CollisionLine* L1 = &C1->Collision.Line;
    struct CollisionLine* L2 = &C2->Collision.Line;

    int x1 = L1->X1, y1 = L1->Y1, x2 = L1->X2, y2 = L1->Y2;
    int x3 = L2->X1, y3 = L2->Y1, x4 = L2->X2, y4 = L2->Y2;

    // Calculate the direction vectors
    int dx1 = x2 - x1, dy1 = y2 - y1;
    int dx2 = x4 - x3, dy2 = y4 - y3;

    // Calculate the determinants
    int det = dx1 * dy2 - dy1 * dx2;
    int det1 = (x3 - x1) * dy2 - (y3 - y1) * dx2;
    int det2 = dx1 * (y3 - y1) - dy1 * (x3 - x1);

    // Check if the lines are not parallel
    if (det != 0)
    {
        // Calculate the intersection point
        float t1 = (float)det1 / det;
        float t2 = (float)det2 / det;

        // Check if the intersection point is within the line segments
        if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)
        {
            return TRUE; // Lines intersect
        }
    }

    return FALSE;   // No intersect
}

/*
    only 1 or 2 active bit combinations allowed, 3 or more is a fail
*/

typedef bool (*CollisionLookupFunction)(struct CollisionObject* C1, struct CollisionObject* C2);

bool (*CollisionLookup[16]) (struct CollisionObject* C1, struct CollisionObject* C2) = 
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

    // At least one object is missing a collider
    if(MaskLeft == 0 || MaskRight == 0)
	    return FALSE;
        
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
