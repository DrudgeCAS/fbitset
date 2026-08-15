/** Tests that the header can be used from more than one translation unit.
 *
 * This file exists mostly so that the header is included by a second
 * translation unit of the test driver.  A header-only library has to link
 * cleanly in that case, which requires every namespace-scope definition in it
 * to have either internal or inline linkage.
 *
 * The iteration test here also walks a bit set all the way to exhaustion for
 * every supported configuration, which is where the iterator used to read one
 * limb past the end of the storage.  That read is only observable under a
 * sanitizer, so the CI has a job building the tests with the address
 * sanitizer.
 */

#include <cstdint>
#include <vector>

#include <fbitset.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace fbitset;

TEST_CASE("Fbitset can be iterated to exhaustion from a second translation unit")
{
    constexpr Size N_BITS = 64;

    auto walk = [N_BITS](auto bits, const std::vector<Size>& idxes) {
        for (auto i : idxes) {
            bits.set(i);
        }

        std::vector<Size> res{};
        for (auto i = bits.begin(); i; ++i) {
            res.push_back(*i);
        }
        CHECK(res == idxes);

        // Walking an empty bit set has to stop straight away.
        decltype(bits) empty(N_BITS);
        CHECK(!empty.begin());
    };

    // A bit in the very last limb of each configuration, so that the iterator
    // has to run off the end of the storage to terminate.
    std::vector<Size> idxes = { 0, 31, 63 };

    walk(Fbitset<1, uint64_t, No_ext>(N_BITS), idxes);
    walk(Fbitset<2, uint32_t, No_ext>(N_BITS), idxes);
    walk(Fbitset<2, uint64_t, std::vector<uint64_t>>(N_BITS), idxes);
    walk(Fbitset<2, uint32_t, std::vector<uint32_t>>(N_BITS), idxes);
    walk(Fbitset<1, uint32_t, std::vector<uint32_t>>(N_BITS), idxes);
    walk(Fbitset<>(N_BITS), idxes);
}
