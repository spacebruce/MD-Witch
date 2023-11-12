#pragma once


typedef void (*StatePtr)();

struct StateType
{
    StatePtr Start;
    StatePtr End;
    StatePtr Tick;
};