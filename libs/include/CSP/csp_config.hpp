#pragma once
//
#define START_NAMESPACE_CSP namespace csp {
#define END_NAMESPACE_CSP }
#define START_NAMESPACE_CSP_PRIVATE namespace priv {
#define END_NAMESPACE_CSP_PRIVATE }
//
#if defined(_MSC_VER)
#define CSP_FINLINE __forceinline
#define CSP_FASTCALL __fastcall
#elif defined(__GNUC__) || defined(__clang__)
#define CSP_FINLINE __attribute__((always_inline)) inline
#define CSP_FASTCALL __attribute__((fastcall))
#else
#define CSP_FINLINE inline
#define MYLIB_FASTCALL
#endif
