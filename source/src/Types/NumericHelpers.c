#include "NumericHelpers.h"

// Clamp function
fix16 fix16Clamp(fix16 value, fix16 min, fix16 max)
{
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}
fix32 fix32Clamp(fix32 value, fix32 min, fix32 max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

// Linear interpolation function
fix16 fix16Lerp(fix16 start, fix16 end, fix16 t)
{
    // Ensure t is in the range [0, FIX16(1)]
    t = fix16Clamp(t, FIX16(0), FIX16(1));
    // Calculate the interpolated value
    fix16 result = start + fix16Mul(t, end - start);
    return result;
}
fix32 fix32Lerp(fix32 start, fix32 end, fix32 t)
{
    // Ensure t is in the range [0, FIX16(1)]
    t = fix32Clamp(t, FIX32(0), FIX32(1));
    // Calculate the interpolated value
    fix32 result = (start + fix32Mul(t, end - start));
    return result;
}

// Why isn't pow() working???
s16 powS16(const s16 value, const s16 pow)
{
    switch(pow)
    {
        case 0: return 0;   break;
        case 1: return value;   break;
    }
    s16 accumulator = value;
    for(int i = 0; i < pow; ++i)
    {
        accumulator *= value;
    }
    return accumulator;
}

s32 powS32(const s32 value, const s32 pow)
{
    switch(pow)
    {
        case 0: return 0;   break;
        case 1: return value;   break;
    }
    s32 accumulator = value;
    for(int i = 0; i < pow; ++i)
    {
        accumulator *= value;
    }
    return accumulator;
}

// NO SQRT?
s16 sqrtS16(const s16 value)
{
    if (value < 0) return -1; // square root of negative number is not defined
    if (value < 2) return value; // square root of 0 is 0, square root of 1 is 1

    s16 start = 0;
    s16 end = value;
    while (start <= end)
    {
        s16 mid = (start + end) / 2;
        s32 sqr = (s32)mid * (s32)mid;
        if (sqr == value)
        {
            return mid;
        }
        else if (sqr < value)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }
    return end;
}

s32 sqrtS32(const s32 value)
{
    if (value < 0) return -1; 
    if (value < 2) return value; 

    s32 start = 0;
    s32 end = value;
    while (start <= end)
    {
        s32 mid = (start + end) / 2;
        s32 sqr = (s32)mid * (s32)mid;
        if (sqr == value)
        {
            return mid;
        }
        else if (sqr < value)
        {
            start = mid + 1;
        }
        else
        {
            end = mid - 1;
        }
    }
    return end;
}