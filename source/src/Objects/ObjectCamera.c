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
}