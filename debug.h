#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef _DEBUG
    #define DBG_PRINT(x) printf x
#else
    #define DBG_PRINT(x) do {} while(0)
#endif

#endif // DEBUG_H
