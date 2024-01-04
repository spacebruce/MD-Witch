#include "ObjectCamera.h"

void ObjectCameraSetTarget(ObjectCamera* Camera, struct ObjectBase* Target)
{
    Camera->Target = Target;
}

void ObjectCameraInit(ObjectCamera* Camera, struct ObjectBase* Target)
{
    if(Target != NULL)  // If target, snap to it
    {
        Camera->Base.x = Target->x;
        Camera->Base.y = Target->y;
        ObjectCameraSetTarget(Camera, Target);
    }
}

void ObjectCameraUpdate(ObjectCamera* Camera)
{
    if(Camera->Target != NULL)
    {
        Camera->Base.x = Camera->Target->x - (320 /2);
        Camera->Base.x = clamp(Camera->Base.x, 0, (Camera->StageWidth) - 320);

        // Give PAL players a little more screen space... they've earned it
        if(IS_PAL_SYSTEM)   
        {
            Camera->Base.y = (Camera->Target->y - 188);
            Camera->Base.y = clamp(Camera->Base.y, 0, (Camera->StageHeight) - 240);
        }
        else
        {
            Camera->Base.y = (Camera->Target->y - 180);
            Camera->Base.y = clamp(Camera->Base.y, 0, (Camera->StageHeight) - 224);
        }
    }
}

void ObjectCameraSetStageSize(ObjectCamera* Camera, const u16 W, const u16 H)
{
    Camera->StageWidth = W;
    Camera->StageHeight = H;
}

void ObjectCameraFreecam(ObjectCamera* Camera, u16 Changed, u16 State)
{
    if((State & BUTTON_LEFT) == BUTTON_LEFT)    Camera->Base.x -= 1;
    if((State & BUTTON_RIGHT) == BUTTON_RIGHT)  Camera->Base.x += 1;
    if((State & BUTTON_UP) == BUTTON_UP)        Camera->Base.y -= 1;
    if((State & BUTTON_DOWN) == BUTTON_DOWN)    Camera->Base.y += 1;
}