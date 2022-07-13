#pragma once
#include <cstdint>

namespace util {
constexpr uint16_t floorlog2(uint16_t x) {
    return x == 1 ? 0 : 1+floorlog2(x >> 1);
}

constexpr uint16_t ceillog2(uint16_t x) {
    return x == 1 ? 0 : floorlog2(x - 1) + 1;
}

#if defined(_MSC_VER) && defined (_WIN64)
#include <intrin.h>// should be part of all recent Visual Studio
#pragma intrinsic(_umul128)
#endif // defined(_MSC_VER) && defined (_WIN64)

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
* Source: https://github.com/lemire/fastrange/blob/master/fastrange.h
*/
static inline uint64_t fastrange64(uint64_t word, uint64_t p) {
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
    return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
#elif defined(_MSC_VER) && defined(_WIN64)
    // supported in Visual Studio 2005 and better
    uint64_t highProduct;
    _umul128(word, p, &highProduct); // ignore output
    return highProduct;
    unsigned __int64 _umul128(
            unsigned __int64 Multiplier,
            unsigned __int64 Multiplicand,
            unsigned __int64 *HighProduct
            );
#else
    return word % p; // fallback
#endif // __SIZEOF_INT128__
}

} // Namespace util
