#pragma once

#include <glm/glm.hpp>

#include "Algorithm.hpp"
#include "Observer.hpp"

namespace Sandbox {

    enum class ControllableAlgorithmCommand : uint8_t {
        None = 0,
        Run,
        Pause,
        Cancel
    };

    template <RandomAccessIteratorConcept Iterator>
    using SwapFunction = std::function<void(
        typename std::iterator_traits<Iterator>::value_type&, typename std::iterator_traits<Iterator>::value_type&
    )>;

    template <RandomAccessIteratorConcept Iterator>
    class ControllableAlgorithm : public IObserver {
    public:
        explicit ControllableAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~ControllableAlgorithm() override = default;

    public:
        void OnNotify(const void* data) override;

    public:
        virtual void operator ()(Iterator begin, Iterator end) const = 0;

    protected:
        [[nodiscard]] inline bool IsRun() const { return m_Command == ControllableAlgorithmCommand::Run; }
        [[nodiscard]] inline bool IsPaused() const { return m_Command == ControllableAlgorithmCommand::Pause; }
        [[nodiscard]] inline bool IsCanceled() const { return m_Command == ControllableAlgorithmCommand::Cancel; }

        [[nodiscard]] bool UpdateState() const;

    protected:
        SwapFunction<Iterator>       m_Swap;
        ControllableAlgorithmCommand m_Command;

    }; // class ControllableAlgorithm

    template <RandomAccessIteratorConcept Iterator, typename AsyncFunction>
    class ControllableParallelAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit ControllableParallelAlgorithm(
            const SwapFunction<Iterator>& swap  = nullptr,
            const AsyncFunction&          async = nullptr
        );
        ~ControllableParallelAlgorithm() override = default;

    protected:
        AsyncFunction m_Async;

    }; // class ControllableAlgorithm

    template <typename Iterator>
    class RandomShuffleAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit RandomShuffleAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~RandomShuffleAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class RandomShuffleAlgorithm

    template <typename Iterator>
    class ReverseAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit ReverseAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~ReverseAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class ReverseAlgorithm

    template <typename Iterator>
    class BubbleSortAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit BubbleSortAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~BubbleSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class BubbleSortQuadsFunctor

    template <typename Iterator>
    class SelectionSortAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit SelectionSortAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~SelectionSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class SelectionSortAlgorithm

    template <typename Iterator>
    class InsertionSortAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit InsertionSortAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~InsertionSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class InsertionSortAlgorithm

    template <typename Iterator>
    class ShellSortAlgorithm : public ControllableAlgorithm<Iterator> {
    public:
        explicit ShellSortAlgorithm(const SwapFunction<Iterator>& swap = nullptr);
        ~ShellSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class ShellSortAlgorithm

    template <typename Iterator>
    class IQuickSortAlgorithm {
    public:
        explicit IQuickSortAlgorithm(ControllableAlgorithmCommand& command, ControllableAlgorithm<Iterator>* innerSort);
        IQuickSortAlgorithm() = default;

    protected:
        [[nodiscard]] Iterator Partition(Iterator begin, Iterator end, const SwapFunction<Iterator>& swap) const;

    protected:
        ControllableAlgorithmCommand&    m_CommandRef;
        ControllableAlgorithm<Iterator>* m_InnerSort;

    }; //class IQuickSortAlgorithm

    template <typename Iterator>
    class QuickSortAlgorithm
        : public ControllableAlgorithm<Iterator>
        , public IQuickSortAlgorithm<Iterator> {
    public:
        explicit QuickSortAlgorithm(const SwapFunction<Iterator>& swap, ControllableAlgorithm<Iterator>* innerSort);
        ~QuickSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class QuickSortAlgorithm

    template <typename Iterator, typename AsyncFunction>
    class ParallelQuickSortAlgorithm
        : public ControllableParallelAlgorithm<Iterator, AsyncFunction>
        , public IQuickSortAlgorithm<Iterator> {
    public:
        explicit ParallelQuickSortAlgorithm(
            const SwapFunction<Iterator>&    swap      = nullptr,
            const AsyncFunction&             async     = nullptr,
            ControllableAlgorithm<Iterator>* innerSort = nullptr
        );
        ~ParallelQuickSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class ParallelQuickSortQuadsFunctor

    template <typename Iterator>
    class IMergeSortAlgorithm {
    public:
        explicit IMergeSortAlgorithm(ControllableAlgorithmCommand& command, ControllableAlgorithm<Iterator>* innerSort);
        virtual ~IMergeSortAlgorithm() = default;

    protected:
        void Merge(Iterator begin, Iterator middle, Iterator end, const SwapFunction<Iterator>& swap) const;
        
    protected:
        ControllableAlgorithmCommand&    m_CommandRef;
        ControllableAlgorithm<Iterator>* m_InnerSort;

    }; // IMergeSortAlgorithm

    template <typename Iterator>
    class MergeSortAlgorithm
        : public ControllableAlgorithm<Iterator>,
        public IMergeSortAlgorithm<Iterator> {
    public:
        explicit MergeSortAlgorithm(
            const SwapFunction<Iterator>&    swap      = nullptr,
            ControllableAlgorithm<Iterator>* innerSort = nullptr
        );
        ~MergeSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class MergeSortAlgorithm

    template <typename Iterator>
    class BottomUpMergeSortAlgorithm
        : public ControllableAlgorithm<Iterator>
        , public IMergeSortAlgorithm<Iterator> {
    public:
        explicit BottomUpMergeSortAlgorithm(
            const SwapFunction<Iterator>&    swap      = nullptr,
            ControllableAlgorithm<Iterator>* innerSort = nullptr
        );
        ~BottomUpMergeSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class BottomUpMergeSortAlgorithm

    template <typename Iterator, typename AsyncFunction>
    class ParallelMergeSortAlgorithm
        : public ControllableParallelAlgorithm<Iterator, AsyncFunction>
        , public IMergeSortAlgorithm<Iterator> {
    public:
        explicit ParallelMergeSortAlgorithm(
            const SwapFunction<Iterator>&    swap      = nullptr,
            const AsyncFunction&             async     = nullptr,
            ControllableAlgorithm<Iterator>* innerSort = nullptr
        );
        ~ParallelMergeSortAlgorithm() override = default;

    public:
        void operator ()(Iterator begin, Iterator end) const override;

    }; // class ParallelMergeSortAlgorithm

} // namespace Sandbox

#include "ControllableAlgorithm.inl"