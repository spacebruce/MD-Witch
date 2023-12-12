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
        Camera->Base.y = (Camera->Target->y - 180);
    }
}

void ObjectCameraFreecam(ObjectCamera* Camera, u16 Previous, u16 Input)
{
    if((Input & BUTTON_LEFT) == BUTTON_LEFT)   Camera->Base.x -= 1;
    if((Input & BUTTON_RIGHT) == BUTTON_RIGHT)  Camera->Base.x += 1;
    if((Input & BUTTON_UP) == BUTTON_UP)        Camera->Base.y -= 1;
    if((Input & BUTTON_DOWN) == BUTTON_DOWN)    Camera->Base.y += 1;
}