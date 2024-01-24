#include <genesis.h>
#include <maths.h>

// Clamp function for fixed-point numbers
fix16 fix16Clamp(fix16 value, fix16 min, fix16 max) {
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
    fix16 result = fix16Add(start, fix16Mul(t, fix16Sub(end, start)));
    return result;
}
fix32 fix32Lerp(fix32 start, fix32 end, fix32 t)
{
    // Ensure t is in the range [0, FIX16(1)]
    t = fix32Clamp(t, FIX32(0), FIX32(1));
    // Calculate the interpolated value
    fix32 result = fix32Add(start, fix32Mul(t, fix32Sub(end, start)));
    return result;
}