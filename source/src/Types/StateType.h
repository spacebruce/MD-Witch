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
} StateType;