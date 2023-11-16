#pragma once

/*
    
*/

enum STATE_ID
{
    STATE_MENU,
    STATE_GAME,
};

//
typedef void (*StateFunction)();

struct StateType
{
    StateFunction Start;
    StateFunction End;
    StateFunction Tick;
};

extern enum STATE_ID STATE_CURRENT;
extern enum STATE_ID STATE_NEXT;
extern struct StateType* StatePtr;