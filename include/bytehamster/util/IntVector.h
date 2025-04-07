#pragma once

#include <vector>
#include <cstdint>

namespace bytehamster::util {
template <uint8_t elementWidth>
class IntVector {
    private:
        static_assert(elementWidth <= 64);
        static constexpr uint64_t MASK = elementWidth == 64 ? ~0ul : ((1ul << elementWidth) - 1);
        size_t numElements;
        std::vector<uint64_t> data;
    public:
        IntVector() : IntVector(0) {
        }

        explicit IntVector(const size_t size) : numElements(size), data((numElements * elementWidth + 63) / 64) {
        }

        void resize(const size_t size) {
            numElements = size;
            data.resize((numElements * elementWidth + 63) / 64);
        }

        [[nodiscard]] uint64_t at(const size_t index) const {
            if constexpr (elementWidth == 0) {
                return 0;
            } else if constexpr (elementWidth == 64) {
                return data[index];
            } else {
                const size_t bitIdx = index * elementWidth;
                const size_t offset = bitIdx % 64;
                const uint64_t* word = &data[bitIdx / 64];
                uint64_t w1 = (*word) >> offset;
                if (offset + elementWidth > 64) {
                    return (w1 | (*(word+1) << (64 - offset))) & MASK;
                } else {
                    return w1 & MASK;
                }
            }
        }

        void set(const size_t index, uint64_t value) {
            if constexpr (elementWidth == 64) {
                data[index] = value;
            } else if constexpr (elementWidth > 0) {
                const size_t bitIdx = index * elementWidth;
                uint64_t* word = &data[bitIdx / 64];
                const size_t offset = bitIdx % 64;
                value &= MASK;
                *word &= ~(MASK << offset);
                *word |= (value << offset);
                if (offset + elementWidth >= 64) {
                    *(word+1) &= ~(MASK >> (64 - offset));
                    *(word+1) |= (value >> (64 - offset));
                }
            }
        }

        [[nodiscard]] size_t dataSizeBytes() const {
            return data.size() * sizeof(uint64_t);
        }

        [[nodiscard]] size_t size() const {
            return numElements;
        }
};
}
