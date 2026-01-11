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

#define t1_disable_copy(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete;

#define t1_disable_move(Class) \
    Class(Class&&) = delete; \
    Class& operator=(Class&&) = delete;

#define t1_disable_copy_and_move(Class) \
    t1_disable_copy(Class) \
    t1_disable_move(Class)
