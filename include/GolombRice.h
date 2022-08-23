#pragma once

#include <vector>
#include <cassert>
#include <sdsl/bit_vectors.hpp>
#include <pasta/bit_vector/bit_vector.hpp>
#include <pasta/bit_vector/support/flat_rank_select.hpp>

namespace util {
/**
 * Compressed integer array.
 * @tparam lowerBits The number of bits to store in the _lower_ part of the data structure.
 */
template <int lowerBits>
class GolombRice {
    static_assert(lowerBits >= 0);
    private:
        sdsl::int_vector<lowerBits> L;
        using ConstIntVector = const sdsl::int_vector<lowerBits>;
        pasta::BitVector H;
        size_t positionInH = 0;
        size_t count = 0;
        pasta::FlatRankSelect<pasta::OptimizedFor::ZERO_QUERIES> *rankSelect = nullptr;
    public:
        explicit GolombRice(size_t num, size_t estimatedAverageValue = 0)
            : L(lowerBits == 0 ? 0 : num), H(((estimatedAverageValue>>lowerBits) + 1) * num, 0) {
        }

        ~GolombRice() {
            invalidateSelectDatastructure();
        }

        void push_back(uint64_t element) {
            uint64_t l = element & ((1l << lowerBits) - 1);
            uint64_t h = element >> lowerBits;
            assert(element == h*(1l << lowerBits) + l);
            if constexpr (lowerBits != 0) {
                L[count] = l;
            }
            if (positionInH + h >= H.size()) {
                std::cout<<"Warning: needed to resize the bit vector in Golomb-Rice coding. "
                         <<"Consider increasing the estimatedAverageValue."<<std::endl;
                size_t oldSize = H.size();
                H.resize(2 * oldSize, 0);
            }
            H[positionInH + h] = 1;
            positionInH += h + 1;
            invalidateSelectDatastructure();
            count++;
        }

        void invalidateSelectDatastructure() {
            delete rankSelect;
        }

        [[nodiscard]] uint64_t at(size_t position) const {
            if (rankSelect == nullptr) {
                throw std::logic_error("Rank/Select not initialized yet. Missing call to buildRankSelect");
            }
            uint64_t l = lowerBits == 0 ? 0 : static_cast<ConstIntVector&>(L)[position];

            size_t positionH = rankSelect->select1(position + 1);
            assert(positionH < H.size());
            uint64_t h = 0;
            while (positionH > 0 && H[positionH-1] == 0) {
                h++;
                positionH--;
            }
            return (h << lowerBits) + l;
        }

        void buildRankSelect() {
            if (rankSelect == nullptr) {
                H.resize(positionInH);
                rankSelect = new pasta::FlatRankSelect<pasta::OptimizedFor::ZERO_QUERIES>(H);
            }
        }

        /**
         * Space usage of this data structure, in bytes.
         */
        [[nodiscard]] size_t space() const {
            return L.capacity()/8 + H.size()/8 + rankSelect->space_usage();
        }
};

} // Namespace util
