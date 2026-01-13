#pragma once
//
#if defined(_MSC_VER)
#define t1_finline      __forceinline
#define t1_finline_cxpr __forceinline constexpr
#elif defined(__GNUC__) || defined(__clang__)
#define t1_finline      __attribute__((always_inline)) inline
#define t1_finline_cxpr __attribute__((always_inline)) inline constexpr
#else
#define t1_finline      inline
#define t1_finline_cxpr inline constexpr
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
