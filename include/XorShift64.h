#pragma once

namespace util {
class XorShift64 {
    private:
        uint64_t x64;
    public:
        explicit XorShift64(uint64_t seed = 88172645463325252ull) : x64(seed) {
        }

        inline uint64_t operator()() {
            x64 ^= x64 << 13;
            x64 ^= x64 >> 7;
            x64 ^= x64 << 17;
            return x64;
        }

        inline uint64_t operator()(uint64_t range) {
            return fastrange64(operator()(), range);
        }
};
}
