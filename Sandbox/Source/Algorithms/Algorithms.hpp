#pragma once

namespace Sandbox {

    class Algorithms {
    public:
        template <typename RandomAccessIterator>
        static void Shuffle(RandomAccessIterator begin, RandomAccessIterator end);

    }; // class Algorithms

} // namespace Sandbox

#include "Algorithms.inl"