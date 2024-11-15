#include <bytehamster/util/EliasFano.h>
#include <bytehamster/util/GolombRice.h>

int main() {
    bytehamster::util::EliasFano<5> eliasFano(5, 100);
    eliasFano.push_back(1);
    eliasFano.push_back(5);
    eliasFano.push_back(20);
    eliasFano.push_back(60);
    eliasFano.push_back(99);
    eliasFano.buildRankSelect();

    // Access
    std::cout << *(eliasFano.at(2)) << std::endl; // 20

    // Access multiple keys without additional select queries
    auto ptr = eliasFano.at(2);
    std::cout << *ptr << std::endl; // 20
    ++ptr;
    std::cout << *ptr << std::endl; // 60

    // Predecessor queries
    std::cout << *(eliasFano.predecessorPosition(70)) << std::endl; // 60

    /////////////////////////////////////////////////

    bytehamster::util::GolombRice<5> golombRice(5, 100);
    golombRice.push_back(10);
    golombRice.push_back(99);
    golombRice.push_back(1);
    golombRice.push_back(60);
    golombRice.push_back(5);
    golombRice.buildRankSelect();

    std::cout << golombRice.at(3) << std::endl; // 60

    /////////////////////////////////////////////////

    constexpr size_t width = 23;
    std::vector<uint64_t> vec1(1000000);
    bytehamster::util::IntVector<width> vec2(vec1.size());
    for (size_t i = 0; i < 3 * vec1.size(); i++) {
        size_t idx = rand() % vec1.size();
        uint64_t val = rand() & ((1ul << width) - 1);
        vec1[idx] = val;
        vec2.set(idx, val);
    }
    for (size_t i = 0; i < vec1.size(); i++) {
        assert(vec1[i] == vec2.at(i));
    }
}