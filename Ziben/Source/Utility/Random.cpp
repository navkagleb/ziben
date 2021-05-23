#include "Random.hpp"

namespace Ziben {

    std::string Random::GetString(std::size_t size, char left, char right) {
        std::string result(size, '\0');

        for (auto& ch : result)
            ch = GetFromRange(left, right);

        return result;
    }

    Random::Data& Random::GetData() {
        static Data data = {
            std::random_device(),
            {
                data.RandomDevice(),
                static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count())
            },
            std::mt19937(data.SeedSequence)
        };

        return data;
    }

} // namespace Ziben