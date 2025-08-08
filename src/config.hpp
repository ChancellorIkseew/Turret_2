#pragma once
//
#if defined(_MSC_VER)
///@brief force inline macro
#define t1_finline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
///@brief force inline macro
#define t1_finline __attribute__((always_inline)) inline
#else
///@brief force inline macro
#define t1_finline inline
#endif
