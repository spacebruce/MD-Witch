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
        Camera->Base.y = 0;//   Camera->Target->y - (224 * (2/3));
    }
}