#include "types.h"
#ifdef NOCRT_CRT_STUBS
    #if defined(_M_IX86)
        int32 _fltused = 0;
        int32 __cdecl _ftol2(real4 x) { return (int32)x; }
        real8 __cdecl _ltod3(int64 x) { return (real8)x; }
        int64 __cdecl _dtol3(real8 x) { return (int64)x; }
        int64 __cdecl _dtol3_sse(real8 x) { return (int64)x; }
        int32 __cdecl _ftol2_sse(real4 x) { return (int32)x; }
        real8 __cdecl _ltod3_sse(int64 x) { return (real8)x; }
        int32 __cdecl __ftol2_sse(real8 x) { return (int32)x; }
        int64 __cdecl _allmul(int64 a, int64 b) { return a * b; }
        int64 __cdecl _allshr(int64 a, uint8 b) { return a >> b; }
        uint64 __cdecl _aullrem(uint64 a, uint64 b) { return a % b; }
        uint64 __cdecl _aulldiv(uint64 a, uint64 b) { return a / b; }
        uint64 __cdecl __aullrem(uint64 a, uint64 b) { return a % b; }
        uint64 __cdecl __aulldiv(uint64 a, uint64 b) { return a / b; }
    #elif defined(_M_X64)
        int32 _fltused = 0;
    #endif
#endif