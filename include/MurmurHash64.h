#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

namespace util {
static inline uint64_t MurmurHash64(const void * key, int len) {
    const uint64_t m = 0xc6a4a7935bd1e995;
    const size_t seed = 1203989050u;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t * data = (const uint64_t *) key;
    const uint64_t * end = data + (len/8);

    while(data != end)
    {
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char * data2 = (const unsigned char*) data;

    switch(len & 7)
    {
        case 7: h ^= uint64_t(data2[6]) << 48; // fallthrough
        case 6: h ^= uint64_t(data2[5]) << 40; // fallthrough
        case 5: h ^= uint64_t(data2[4]) << 32; // fallthrough
        case 4: h ^= uint64_t(data2[3]) << 24; // fallthrough
        case 3: h ^= uint64_t(data2[2]) << 16; // fallthrough
        case 2: h ^= uint64_t(data2[1]) << 8;  // fallthrough
        case 1: h ^= uint64_t(data2[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

static inline uint64_t MurmurHash64(uint64_t key) {
    return MurmurHash64(&key, sizeof(key));
}

static inline uint64_t MurmurHash64(const std::string &key) {
    return MurmurHash64(key.data(), key.size());
}

static inline uint64_t MurmurHash64Seeded(uint64_t key, uint64_t seed) {
    return MurmurHash64(MurmurHash64(key) ^ seed);
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
