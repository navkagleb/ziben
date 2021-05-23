#include <Ziben/Utility/Random.hpp>

namespace Sandbox {

    template <RandomAccessIteratorConcept T>
    void Algorithm::Shuffle(T begin, T end) {
        auto distance = end - begin;

        for (auto i = distance - 1; i > 0; --i) {
            auto position = Ziben::Random::GetFromRange<decltype(i)>(0, i);

            std::swap(*std::next(begin, i), *std::next(begin, position));
        }
    }

    template <RandomAccessIteratorConcept T, typename Func>
    void Algorithm::Shuffle(T begin, T end, Func func) {
        auto distance = end - begin;

        for (auto i = distance - 1; i > 0; --i)
            std::swap(*std::next(begin, i), *std::next(begin, func(i)));
    }

    template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator>
    void Algorithm::BubbleSort(T begin, T end, Comparator comparator) {
        bool isSwapped;

        for (auto i = begin; i != end - 1; ++i) {
            isSwapped = false;

            for (auto j = begin; j != end - std::distance(begin, i) - 1; ++j) {
                if (comparator(*(j + 1), *j)) {
                    isSwapped = true;
                    std::swap(*(j + 1), *j);
                }
            }

            if (!isSwapped)
                break;
        }
    }

    template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator>
    void Algorithm::SelectionSort(T begin, T end, Comparator comparator) {
        for (auto i = begin, min = T(); i != end - 1; ++i) {
            min = i;

            for (auto j = i; j < end; ++j)
                if (comparator(*j, *min))
                    min = j;

            std::swap(*i, *min);
        }
    }

    template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator>
    void Algorithm::InsertionSort(T begin, T end, Comparator comparator) {
        for (auto i = begin + 1; i != end; ++i)
            for (auto j = i; j != begin && comparator(*j, *(j - 1)); --j)
                std::swap(*j, *(j - 1));
    }

    template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator>
    void Algorithm::ShellSort(T begin, T end, Comparator comparator) {
        typename std::iterator_traits<T>::difference_type h = 1;

        while (h < (end - begin) / 3)
            h = 3 * h + 1;

        while (h >= 1) {
            for (auto i = begin + h; i != end; ++i)
                for (auto j = i; j >= begin + h && comparator(*j, *(j - h)); j -= h)
                    std::swap(*j, *(j - h));

            h = h / 3;
        }
    }

    template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator>
    void Algorithm::QuickSort(T begin, T end, Comparator comparator) {
        if (begin == end)
            return;

//        if (end <= begin + 5)
//            return InsertionSort(begin, end, comparator);

        auto partition = Partition(begin, end - 1, comparator);

        QuickSort(begin,         partition, comparator);
        QuickSort(partition + 1, end,           comparator);
    }

    template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator>
    T Algorithm::Partition(T begin, T end, Comparator comparator) {
        auto i = begin - 1;
        auto j = end;

        while (true) {
            while (comparator(*(++i), *end))
                if (i == end)
                    break;

            while (comparator(*end, *(--j)))
                if (j == begin)
                    break;

            if (j <= i)
                break;

            std::swap(*i, *j);
        }

        std::swap(*end, *i);

        return i;
    }

} // namespace Sandbox