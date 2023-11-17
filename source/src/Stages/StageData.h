#pragma once

/*
    Since stages have their own logic, they need somewhere to store it... 
    Keeping it every stage open in RAM at once is a huge waste, my approach is to use a union containing every stage's data and make sure it's init correctly on stage load.
*/

// Relocate all to seperate headers later if this gets unwieldy
typedef struct 
{
    s8 Something
} Stage11Data;

typedef struct {} Stage12Data;
typedef struct {} Stage13Data;

union 
{
    Stage11Data Stage11;
    Stage12Data Stage12;
    Stage13Data Stage13;
} StageData;    