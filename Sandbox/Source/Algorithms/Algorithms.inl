#include <Ziben/Utility/Random.hpp>

namespace Sandbox {

    template <typename RandomAccessIterator>
    void Algorithms::Shuffle(RandomAccessIterator begin, RandomAccessIterator end) {
        auto distance = std::distance(begin, end);

        for (auto i = distance - 1; i > 0; --i) {
            auto position = Ziben::Random::GetRef().Get<decltype(i)>(0, i);

            std::swap(*std::next(begin, i), *std::next(begin, position));
        }
    }

} // namespace Sandbox