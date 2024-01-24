#include "ObjectCamera.h"

#include "../Types/FixedPointHelpers.h"

void ObjectCameraSetTarget(ObjectCamera* Camera, struct ObjectBase* Target)
{
    Camera->Target = Target;
}

void ObjectCameraInit(void* object)
{
    ObjectCamera* Camera = (ObjectCamera*)object;
    Camera->Target = NULL;
}

void ObjectCameraUpdate(void* object)
{
    ObjectCamera* Camera = (ObjectCamera*)object;
    if(Camera->Target != NULL)
    {
        fix32 x = Camera->Base.x, y = Camera->Base.y;
        x = fix32Sub(Camera->Target->x, FIX32(320 /2));
        int cx = fix32ToInt(x);
        if(cx < 0)
        {
            x = FIX32(0);
        }
        else if (cx > (Camera->StageWidth - 320))
        {
            x = FIX32(Camera->StageWidth - 320);
        }

        fix32 minus;
        s16 border;
        // Give PAL players a little more screen space... they've earned it
        if(IS_PAL_SYSTEM)   
        {
            minus = FIX32(188);
            border = 240;
        }
        else    // NTSC has a slightly shorter window
        {
            minus = FIX32(180);
            border = 224;
        }
        
        y = fix32Sub(Camera->Target->y, minus);
        int cy = fix32ToInt(y);
        if(cy < 0)
        {
            y = FIX32(0);
        }
        else if (cy > (Camera->StageHeight - border))
        {
            y = FIX32(Camera->StageWidth - border);
        }

        //
        Camera->Base.x = fix32Lerp(Camera->Base.x, x, FIX32(0.15));
        Camera->Base.y = fix32Lerp(Camera->Base.y, y, FIX32(0.15));
    }
}

void ObjectCameraSetStageSize(ObjectCamera* Camera, const u16 W, const u16 H)
{
    Camera->StageWidth = W;
    Camera->StageHeight = H;
}

void ObjectCameraFreecam(ObjectCamera* Camera, u16 State)
{

    if((State & BUTTON_LEFT) == BUTTON_LEFT)    
        Camera->Base.x = fix32Sub(Camera->Base.x, FIX32(1));
    if((State & BUTTON_RIGHT) == BUTTON_RIGHT) 
        Camera->Base.x = fix32Add(Camera->Base.x, FIX32(1));
    if((State & BUTTON_UP) == BUTTON_UP)
        Camera->Base.y = fix32Sub(Camera->Base.y, FIX32(1));
    if((State & BUTTON_DOWN) == BUTTON_DOWN)
        Camera->Base.y = fix32Add(Camera->Base.y, FIX32(1));
}