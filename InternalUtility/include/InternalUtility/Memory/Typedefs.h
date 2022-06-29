#pragma once

#if !defined(PLATFORM32) && !defined(PLATFORM64)
#error "A platform must be specified by defining PLATFORM32 or PLATFORM64"
#endif

#if defined (PLATFORM32) && defined(PLATFORM64)
#error "Both platforms are defined"
#endif

#ifdef PLATFORM32
typedef __int32 BaseType_t;
#elif defined(PLATFORM64)
typedef __int64 BaseType_t;
#endif
