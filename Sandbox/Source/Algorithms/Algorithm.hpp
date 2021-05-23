#pragma once

#include <iterator>

namespace Sandbox {

    template <typename T>
    concept RandomAccessIteratorConcept = std::is_same_v<
        typename std::iterator_traits<T>::iterator_category,
        std::random_access_iterator_tag
    >;

    template <typename Comparator, typename T>
    concept ComparatorConcept = requires (Comparator comparator, typename std::iterator_traits<T>::value_type t) {
        { comparator(t, t) } -> std::convertible_to<bool>;
    };

    class Algorithm {
    public:
        template <RandomAccessIteratorConcept T>
        static void Shuffle(T begin, T end);

        template <RandomAccessIteratorConcept T, typename Func>
        static void Shuffle(T begin, T end, Func func);

        template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator = std::less<>()>
        static void BubbleSort(T begin, T end, Comparator comparator);

        template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator = std::less<>()>
        static void SelectionSort(T begin, T end, Comparator comparator);

        template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator = std::less<>()>
        static void InsertionSort(T begin, T end, Comparator comparator);

        template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator = std::less<>()>
        static void ShellSort(T begin, T end, Comparator comparator);

        template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator = std::less<>()>
        static void QuickSort(T begin, T end, Comparator comparator);

    private:
        template <RandomAccessIteratorConcept T, ComparatorConcept<T> Comparator = std::less<>()>
        static T Partition(T begin, T end, Comparator comparator);

    }; // class Algorithm

} // namespace Sandbox

#include "Algorithm.inl"