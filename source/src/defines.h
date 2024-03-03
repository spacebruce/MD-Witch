#pragma once

#define DEBUG_MODE true

#if(DEBUG_MODE)
#define DEBUG_ATTACK true
#define DEBUG_PERF true
#else
#define DEBUG_ATTACK false
#define DEBUG_PERF false
#endif

#define PAL_BACKGROUND PAL0
#define PAL_TILES PAL1
#define PAL_PLAYER PAL2
#define PAL_ENEMIES PAL3