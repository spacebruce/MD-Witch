#include "ObjectManager.h"

#include "GameContext.h"

// 8KB
//#define ObjectPoolSize (6 * 1024)
#define ObjectPoolSlots (80)

struct ObjectTracker
{
    void* Start;
    ObjectID Type;
};

// Memory pool state
//static char ObjectPool[ObjectPoolSize];
//static uint16_t ObjectPointer;
static struct ObjectTracker Objects[ObjectPoolSlots];
static uint8_t ActiveObjectID;
static uint8_t ActiveObjectCount;

void InitObjectManager()
{
    //memset(ObjectPool, '\0', ObjectPoolSize);
    //ObjectPointer = 0;
    ActiveObjectID = 0;
    ActiveObjectCount = 0;
    for(int i = 0; i < ObjectPoolSlots; ++i)
    {
        Objects[i].Start = NULL;
    }
}

void EndObjectManager()
{
    // nuke everything, just in case
    for(int i = 0; i < ObjectPoolSlots; ++i)
        DestroyObject(i);
}

void* CreateObject(const ObjectID Type, fix32 X, fix32 Y)
{
    if(ActiveObjectCount == ObjectPoolSlots)
    {
        kprintf("No object slots available");
        return NULL;
    }

    // Look up construction data for object
    const struct ObjectManifest* meta = &ObjectList[Type]; 

    // Grab memory
    struct ObjectBase* thing = MEM_alloc(meta->Size);
    if(thing == NULL)
    {
        kprintf("Cannot alloc memory for object %s", meta->name);
        return NULL;
    }

    // Find slot for object to go in
    if(Objects[ActiveObjectID].Start != NULL)
    {
        while(Objects[ActiveObjectID].Start != NULL)
        {
            ActiveObjectID = (ActiveObjectID + 1) % ObjectPoolSlots;
        }
    }

    // Construct object in place
    ObjectBaseInit((void*)thing, X,Y,Type);
    if(meta->Init != NULL)
    {
        meta->Init((void*)thing);
    }
    thing->ActiveObjectID = ActiveObjectID;
    thing->ObjectType = Type;

    // Keep track of it
    Objects[ActiveObjectID].Start = thing;
    Objects[ActiveObjectID].Type = Type;

    // 
    ++ActiveObjectID;
    ++ActiveObjectCount;

    return thing;
}

void DestroyObject(const uint8_t ID)
{
    ObjectID Type = Objects[ID].Type;
    const struct ObjectManifest* meta = &ObjectList[Type]; 

    // Call dtor
    if(meta->Kill != NULL)
        meta->Kill(Objects[ID].Start);
    
    // Free memory
    MEM_free(Objects[ID].Start);
    Objects[ID].Start = NULL;

    if(ActiveObjectID > ID)
    {
        ActiveObjectID = ID;
    }
    --ActiveObjectCount;
}

void TickObjects()
{
    uint8_t processed = 0, index = 0;
    while(processed < ActiveObjectCount)
    {
        // Skip gaps
        while(Objects[index].Start == NULL)
        {
            ++index;
        }
        // grab object properties from type
        const struct ObjectManifest* meta = &ObjectList[Objects[index].Type];
        // update
        meta->Tick(Objects[index].Start);
        ObjectUpdateSprite(Objects[index].Start, GameContext.Camera->Base.x, GameContext.Camera->Base.y);
        // next
        ++processed;
        ++index;
    }
}

void* GetObjectFromID(const uint8_t ID)
{
    if(ID >= ObjectPoolSlots)
        return NULL;
    return Objects[ID].Start;
}