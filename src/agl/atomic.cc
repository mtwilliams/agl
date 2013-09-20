/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#include <agl/atomic.h>

#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #define WIN32_LEAN_AND_MEAN
  #define WIN32_EXTRA_LEAN
  #include <Windows.h>
  #undef WIN32_EXTRA_LEAN
  #undef WIN32_LEAN_AND_MEAN

  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #include <intrin.h>
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      #pragma intrinsic(_InterlockedIncrement)
      #pragma intrinsic(_InterlockedDecrement)
      #pragma intrinsic(_InterlockedCompareExchange)
      #pragma intrinsic(_InterlockedCompareExchangePointer)
      #pragma intrinsic(_interlockedbittestandset)
      #pragma intrinsic(_interlockedbittestandreset)
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      #pragma intrinsic(_InterlockedIncrement64)
      #pragma intrinsic(_InterlockedDecrement64)
      #pragma intrinsic(_InterlockedCompareExchange64)
      #pragma intrinsic(_InterlockedCompareExchangePointer)
      #pragma intrinsic(_interlockedbittestandset64)
      #pragma intrinsic(_interlockedbittestandreset64)
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      /* InterlockedIncrement */
      /* InterlockedDecrement */
      /* InterlockedCompareExchange */
      /* InterlockedCompareExchangePointer */
      /* InterlockedBitTestAndSet */
      /* InterlockedBitTestAndReset */
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      /* InterlockedIncrement64 */
      /* InterlockedDecrement64 */
      /* InterlockedCompareExchange64 */
      /* InterlockedCompareExchangePointer */
      /* InterlockedBitTestAndSet64 */
      /* InterlockedBitTestAndReset64 */
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    /* See "5.47 Built-in functions for atomic memory access" */
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ==========================================================================
    Atomics (agl_atomic_incr, agl_atomic_decr, agl_atomic_compr_and_swap):
   ========================================================================== */

uint agl_atomic_incr(
  volatile uint *value)
{
#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return _InterlockedIncrement((volatile LONG *)value);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return _InterlockedIncrement64((volatile LONG64 *)value);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return InterlockedIncrement((volatile LONG *)value);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return InterlockedIncrement64((volatile LONG64 *)value);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    return __sync_add_and_fetch(value, 1);
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif
}

uint agl_atomic_decr(
  volatile uint *value)
{
#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return _InterlockedDecrement((volatile LONG *)value);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return _InterlockedDecrement64((volatile LONG64 *)value);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return InterlockedDecrement((volatile LONG *)value);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return InterlockedDecrement64((volatile LONG64 *)value);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    return __sync_sub_and_fetch(value, 1);
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif
}

uint agl_atomic_compr_and_swap(
  volatile uint *comparee,
  const uint comparand,
  const uint value)
{
#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return _InterlockedCompareExchange(
        (volatile LONG *)comparee, value, comparand);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return _InterlockedCompareExchange64(
        (volatile LONG64 *)comparee, value, comparand);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return InterlockedCompareExchange(
        (volatile LONG *)comparee, value, comparand);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return InterlockedCompareExchange64(
        (volatile LONG64 *)comparee, value, comparand);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    return __sync_val_compare_and_swap(comparee, comparand, value);
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif
}

uintptr_t agl_atomic_compr_and_swap_ptr(
  volatile uintptr_t *comparee,
  const uintptr_t comparand,
  const uintptr_t value)
{
#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return (uintptr_t)_InterlockedCompareExchangePointer(
        (volatile void *)comparee, (void *)value, (void *)comparand);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return (uintptr_t)_InterlockedCompareExchangePointer(
        (volatile void *)comparee, value, comparand);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return (uintptr_t)InterlockedCompareExchangePointer(
        (volatile void *)comparee, (void *)value, (void *)comparand);
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return (uintptr_t)InterlockedCompareExchangePointer(
        (volatile void *)comparee, (void *)value, (void *)comparand);
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    return __sync_val_compare_and_swap(comparee, comparand, value);
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif
}

bool agl_atomic_test_and_set(
  volatile uint *comparee,
  const uint bit)
{
#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return !!(_interlockedbittestandset(
        (volatile LONG *)comparee, bit));
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return !!(_interlockedbittestandset64(
        (volatile LONG64 *)comparee, bit));
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    const uint mask = (1u << bit);
    uint v; do {
      v = *comparee;
    } while (agl_atomic_compr_and_swap(comparee, v, v & mask) != v);
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    const uint mask = (1u << bit);
    uint v; do {
      v = *comparee;
    } while(!__sync_bool_compare_and_swap(comparee, v, v & mask));
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif
}

bool agl_atomic_test_and_clear(
  volatile uint *comparee,
  const uint bit)
{
#if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
  #if (AGL_COMPILER == AGL_COMPILER_MSVC)
    #if (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86)
      return !!(_interlockedbittestandreset(
        (volatile LONG *)comparee, bit));
    #elif (AGL_ARCHITECTURE == AGL_ARCHITECTURE_X86_64)
      return !!(_interlockedbittestandreset64(
        (volatile LONG64 *)comparee, bit));
    #else
      #error ("Unknown or unsupported architecture!")
    #endif
  #else
    const uint mask = ~(1u << bit);
    uint v; do {
      v = *comparee;
    } while (agl_atomic_compr_and_swap(comparee, v, v & mask) != v);
  #endif
#elif AGL_PLATFORM_IS_POSIX(AGL_PLATFORM)
  #if ((AGL_COMPILER == AGL_COMPILER_GCC) || (AGL_COMPILER == AGL_COMPILER_CLANG))
    const uint mask = ~(1u << bit);
    uint v; do {
      v = *comparee;
    } while(!__sync_bool_compare_and_swap(comparee, v, v & mask));
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#else
  #error ("Unknown or unsupported platform!")
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
