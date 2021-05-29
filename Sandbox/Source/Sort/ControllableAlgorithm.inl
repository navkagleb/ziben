#include <Ziben/Utility/Random.hpp>

namespace Sandbox {

    template <RandomAccessIteratorConcept Iterator>
    ControllableAlgorithm<Iterator>::ControllableAlgorithm(const SwapFunction<Iterator>& swap)
        : m_Swap(swap)
        , m_Command(ControllableAlgorithmCommand::None) {}

    template <RandomAccessIteratorConcept Iterator>
    void ControllableAlgorithm<Iterator>::OnNotify(const void* data) {
        m_Command = *reinterpret_cast<const ControllableAlgorithmCommand*>(data);
    }

    template <RandomAccessIteratorConcept Iterator>
    bool ControllableAlgorithm<Iterator>::UpdateState() const {
        return true;
    }

    template <RandomAccessIteratorConcept Iterator, typename AsyncFunction>
    ControllableParallelAlgorithm<Iterator, AsyncFunction>::ControllableParallelAlgorithm(
        const SwapFunction<Iterator>& swap,
        const AsyncFunction&          async
    )
        : ControllableAlgorithm<Iterator>(swap)
        , m_Async(async) {}

    template <typename Iterator>
    RandomShuffleAlgorithm<Iterator>::RandomShuffleAlgorithm(const SwapFunction<Iterator>& swap)
        : ControllableAlgorithm<Iterator>(swap) {}

    template <typename Iterator>
    void RandomShuffleAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        using DifferenceType = typename std::iterator_traits<Iterator>::difference_type;

        for (auto i = begin; i != end; ++i) {
            while (this->IsPaused())
                std::this_thread::yield();

            if (this->IsCanceled())
                return;

            this->m_Swap(*i, *(begin + Ziben::Random::GetFromRange<DifferenceType>(i - begin, end - begin - 1)));
        }
    }

    template <typename Iterator>
    ReverseAlgorithm<Iterator>::ReverseAlgorithm(const SwapFunction<Iterator>& swap)
        : ControllableAlgorithm<Iterator>(swap) {}

    template <typename Iterator>
    void ReverseAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        for (auto i = begin; i < begin + (end - begin) / 2; ++i) {
            while (this->IsPaused())
                std::this_thread::yield();

            if (this->IsCanceled())
                return;

            this->m_Swap(*i, *(end - (i - begin) - 1));
        }
    }

    template <typename Iterator>
    BubbleSortAlgorithm<Iterator>::BubbleSortAlgorithm(const SwapFunction<Iterator>& swap)
        : ControllableAlgorithm<Iterator>(swap) {}

    template <typename Iterator>
    void BubbleSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        bool isSwapped;

        for (auto i = begin; i != end - 1; ++i) {
            isSwapped = false;

            for (auto j = begin; j != end - std::distance(begin, i) - 1; ++j) {
                while (this->IsPaused())
                    std::this_thread::yield();

                if (this->IsCanceled())
                    return;

                if (*(j + 1) < *j) {
                    isSwapped = true;

                    this->m_Swap(*(j + 1), *j);
                }
            }

            if (!isSwapped)
                break;
        }
    }

    template <typename Iterator>
    SelectionSortAlgorithm<Iterator>::SelectionSortAlgorithm(const SwapFunction<Iterator>& swap)
        : ControllableAlgorithm<Iterator>(swap) {}

    template <typename Iterator>
    void SelectionSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        for (auto i = begin, min = begin; i != end - 1; ++i) {
            min = i;

            for (auto j = i; j < end; ++j) {
                while (this->IsPaused())
                    std::this_thread::yield();

                if (this->IsCanceled())
                    return;

                if (*j < *min)
                    min = j;
            }

            this->m_Swap(*i, *min);
        }
    }

    template <typename Iterator>
    InsertionSortAlgorithm<Iterator>::InsertionSortAlgorithm(const SwapFunction<Iterator>& swap)
        : ControllableAlgorithm<Iterator>(swap) {}

    template <typename Iterator>
    void InsertionSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        for (auto i = begin + 1; i != end; ++i) {
            for (auto j = i; j != begin && *j < *(j - 1); --j) {
                while (this->IsPaused())
                    std::this_thread::yield();

                if (this->IsCanceled())
                    return;

                this->m_Swap(*j, *(j - 1));
            }
        }
    }

    template <typename Iterator>
    ShellSortAlgorithm<Iterator>::ShellSortAlgorithm(const SwapFunction<Iterator>& swap)
        : ControllableAlgorithm<Iterator>(swap) {}

    template <typename Iterator>
    void ShellSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        using DifferenceType = typename std::iterator_traits<Iterator>::difference_type;

        DifferenceType h = 1;

        while (h < (end - begin) / 3)
            h = 3 * h + 1;

        while (h >= 1) {
            for (auto i = begin + h; i != end; ++i) {
                for (auto j = i; j >= begin + h && *j < *(j - h); j -= h) {
                    while (this->IsPaused())
                        std::this_thread::yield();

                    if (this->IsCanceled())
                        return;

                    this->m_Swap(*j, *(j - h));
                }
            }

            h = h / 3;
        }
    }

    template <typename Iterator>
    IQuickSortAlgorithm<Iterator>::IQuickSortAlgorithm(
        ControllableAlgorithmCommand&    command,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : m_CommandRef(command)
        , m_InnerSort(innerSort) {}

    template <typename Iterator>
    Iterator IQuickSortAlgorithm<Iterator>::Partition(
        Iterator                      begin,
        Iterator                      end,
        const SwapFunction<Iterator>& swap
    ) const {
        auto i = begin - 1;
        auto j = end;

        while (true) {
            while (this->m_CommandRef == ControllableAlgorithmCommand::Pause)
                std::this_thread::yield();

            if (this->m_CommandRef == ControllableAlgorithmCommand::Cancel)
                return end + 1;

            while (*(++i) < *end)
                if (i == end)
                    break;

            while (*end < *(--j))
                if (j == begin)
                    break;

            if (j <= i)
                break;

            swap(*i, *j);
        }

        swap(*end, *i);

        return i;
    }

    template <typename Iterator>
    QuickSortAlgorithm<Iterator>::QuickSortAlgorithm(
        const SwapFunction<Iterator>&    swap,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : ControllableAlgorithm<Iterator>(swap)
        , IQuickSortAlgorithm<Iterator>(this->m_Command, innerSort) {}

    template <typename Iterator>
    void QuickSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return (*this->m_InnerSort)(begin, end);

        auto partition = this->Partition(begin, end - 1, this->m_Swap);

        if (partition == end)
            return;

        (*this)(begin,         partition);
        (*this)(partition + 1, end      );
    }

    template <typename Iterator, typename AsyncFunction>
    ParallelQuickSortAlgorithm<Iterator, AsyncFunction>::ParallelQuickSortAlgorithm(
        const SwapFunction<Iterator>&    swap,
        const AsyncFunction&             async,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : ControllableParallelAlgorithm<Iterator, AsyncFunction>(swap, async)
        , IQuickSortAlgorithm<Iterator>(this->m_Command, innerSort) {}

    template <typename Iterator, typename AsyncFunction>
    void ParallelQuickSortAlgorithm<Iterator, AsyncFunction>::operator ()(Iterator begin, Iterator end) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return (*this->m_InnerSort)(begin, end);

        auto partition = this->Partition(begin, end - 1, this->m_Swap);

        if (partition == end)
            return;

        if (end - begin >= 20) {
            auto left = this->m_Async([&] { return (*this)(begin, partition); });
            (*this)(partition + 1, end);

            left.wait();
        } else {
            (*this)(begin,         partition);
            (*this)(partition + 1, end      );
        }
    }

    template <typename Iterator>
    IMergeSortAlgorithm<Iterator>::IMergeSortAlgorithm(
        ControllableAlgorithmCommand&    command,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : m_CommandRef(command)
        , m_InnerSort(innerSort) {}
        
    template <typename Iterator>
    void IMergeSortAlgorithm<Iterator>::Merge(
        Iterator                      begin,
        Iterator                      middle,
        Iterator                      end,
        const SwapFunction<Iterator>& swap
    ) const {
        using ValueType = typename std::iterator_traits<Iterator>::value_type;

        std::vector<ValueType> temp(std::distance(begin, end));

        auto i = begin;
        auto j = middle;

        for (auto& k : temp) {
            while (this->m_CommandRef == ControllableAlgorithmCommand::Pause)
                std::this_thread::yield();

            if (this->m_CommandRef == ControllableAlgorithmCommand::Cancel)
                return;

            if ((*i < *j && i < middle) || j >= end)
                k = *(i++);
            else if ((!(*i < *j) && j < end) || i >= middle)
                k = *(j++);
        }

        for (auto k = temp.begin(); k != temp.end(); ++k)
            swap(*(begin + std::distance(temp.begin(), k)), *k);
    }

    template <typename Iterator>
    MergeSortAlgorithm<Iterator>::MergeSortAlgorithm(
        const SwapFunction<Iterator>&    swap,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : ControllableAlgorithm<Iterator>(swap)
        , IMergeSortAlgorithm<Iterator>(this->m_Command, innerSort) {}

    template <typename Iterator>
    void MergeSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return (*this->m_InnerSort)(begin, end);

        auto middle = begin + std::distance(begin, end) / 2;

        (*this)(begin,  middle);
        (*this)(middle, end   );

        this->Merge(begin, middle, end, this->m_Swap);
    }        
        
    template <typename Iterator>
    BottomUpMergeSortAlgorithm<Iterator>::BottomUpMergeSortAlgorithm(
        const SwapFunction<Iterator>&    swap,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : ControllableAlgorithm<Iterator>(swap)
        , IMergeSortAlgorithm<Iterator>(this->m_Command, innerSort) {}
        
    template <typename Iterator>
    void BottomUpMergeSortAlgorithm<Iterator>::operator ()(Iterator begin, Iterator end) const {
        using DifferenceType = typename std::iterator_traits<Iterator>::difference_type;
        
        for (DifferenceType size = 1; size < end - begin; size += size)
            for (auto left = begin; left < end - size; left += size * 2)
                this->Merge(left, left + size, std::min(left + size + size, end), this->m_Swap);
    }

    template <typename Iterator, typename AsyncFunction>
    ParallelMergeSortAlgorithm<Iterator, AsyncFunction>::ParallelMergeSortAlgorithm(
        const SwapFunction<Iterator>&    swap,
        const AsyncFunction&             async,
        ControllableAlgorithm<Iterator>* innerSort
    )
        : ControllableParallelAlgorithm<Iterator, AsyncFunction>(swap, async)
        , IMergeSortAlgorithm<Iterator>(this->m_Command, innerSort) {}

    template <typename Iterator, typename AsyncFunction>
    void ParallelMergeSortAlgorithm<Iterator, AsyncFunction>::operator ()(Iterator begin, Iterator end) const {
        if (begin >= end)
            return;

        if (auto distance = std::distance(begin, end); distance <= 5 && distance >= 2)
            return (*this->m_InnerSort)(begin, end);

        auto middle = begin + std::distance(begin, end) / 2;

        if (end - begin >= 20) {
            auto left = this->m_Async([&] { return (*this)(begin, middle); });
            (*this)(middle, end);

            left.wait();
        } else {
            (*this)(begin,  middle);
            (*this)(middle, end   );
        }

        this->Merge(begin, middle, end, this->m_Swap);
    }
    
} // namespace Sandbox