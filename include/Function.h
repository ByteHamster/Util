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
    return word / (UINT64_MAX / p); // fallback
#endif // __SIZEOF_INT128__
}

/**
* Given a value "word", produces an integer in [0,p) without division.
* The function is as fair as possible in the sense that if you iterate
* through all possible values of "word", then you will generate all
* possible outputs as uniformly as possible.
*/
static inline uint32_t fastrange32(uint32_t word, uint32_t p) {
    return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
}

template <uint64_t p>
static inline uint32_t fastrange32(uint32_t word) {
    return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
}

static inline uint64_t fastrange16(uint64_t x, uint64_t n) {
    static const int masklen = 48;
    static const uint64_t mask = (uint64_t(1) << masklen) - 1;
    return ((x & mask) * n) >> masklen;
}

/** David Stafford's (http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html)
 * 13th variant of the 64-bit finalizer function in Austin Appleby's
 * MurmurHash3 (https://github.com/aappleby/smhasher).
 *
 * @param z a 64-bit integer.
 * @return a 64-bit integer obtained by mixing the bits of `z`.
 */
uint64_t inline remix(uint64_t z) {
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}
} // Namespace util
