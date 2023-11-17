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

typedef struct
{
    StateFunction Start;
    StateFunction End;
    StateFunction Tick;
    StateFunction EndTick;
} StateType;

extern enum STATE_ID STATE_CURRENT;
extern enum STATE_ID STATE_NEXT;
extern StateType* StatePtr;