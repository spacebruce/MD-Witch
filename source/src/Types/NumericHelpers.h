#include <genesis.h>
#include <maths.h>

/*
    for when I need a number thing and the library isn't helping
*/

// My kingdom for C++ templates, I miss them. 

// Clamp function
fix16 fix16Clamp(fix16 value, fix16 min, fix16 max);
fix32 fix32Clamp(fix32 value, fix32 min, fix32 max);

// Linear interpolation function
fix16 fix16Lerp(fix16 start, fix16 end, fix16 t);
fix32 fix32Lerp(fix32 start, fix32 end, fix32 t);

// Why isn't pow() working???
s16 powS16(const s16 value, const s16 pow);
s32 powS32(const s32 value, const s32 pow);

// NO SQRT???
int16_t sqrtS16(const int16_t value);
int32_t sqrtS32(const int32_t value);