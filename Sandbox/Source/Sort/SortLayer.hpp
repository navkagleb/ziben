#pragma once

#include <future>
#include <atomic>

#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCamera.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>

#include "ParticleSystem.hpp"
#include "ControllableAlgorithm.hpp"
#include "SortableQuad.hpp"

namespace Sandbox {

    class SortLayer : public Ziben::Layer, public Subject {
    public:
        SortLayer();
        ~SortLayer() noexcept override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Ziben::Event& event) override;
        void OnUpdate(const Ziben::TimeStep& ts) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        enum class ShuffleType : uint8_t {
            None = 0,
            RandomShuffle,
            Reverse
        };

        enum class SortType : uint8_t {
            None = 0,
            BubbleSort,
            SelectionSort,
            InsertionSort,
            ShellSort,
            QuickSort,
            ParallelQuickSort,
            MergeSort,
            BottomUpMergeSort,
            ParallelMergeSort
        };

    private:
        using ControllableAlgorithm      = ControllableAlgorithm<QuadIterator>;

        using ShuffleAlgorithmContainer = std::unordered_map<ShuffleType, ControllableAlgorithm*>;
        using SortAlgorithmContainer     = std::unordered_map<SortType, ControllableAlgorithm*>;

    private:
        bool OnWindowResized(Ziben::WindowResizedEvent& event);
        bool OnWindowClosed(Ziben::WindowClosedEvent& event);

        ControllableAlgorithm* CreateShuffleAlgorithm(ShuffleType type);
        ControllableAlgorithm* CreateSortAlgorithm(SortType type);

        void InitQuads(std::size_t count);
        void UpdateQuads(std::size_t count = 0);
        void ResetQuads();
        void SwapQuads(Quad& lhs, Quad& rhs);

        template <typename Function, typename... Args>
        std::future<void> AsyncRun(Function&& function, Args&&... args);

        template <ShuffleType ShuffleType>
        void Shuffle();

        template <SortType SortType>
        void Sort();

        void BloomParticle();

    private:
        Ziben::OrthographicCamera      m_Camera;
        Ziben::Ref<Ziben::FrameBuffer> m_FrameBuffer;
        glm::vec<2, uint32_t>          m_ViewportSize;
        std::array<glm::vec2, 2>       m_ViewportBounds;

        ShuffleAlgorithmContainer      m_ShuffleAlgorithms;
        SortAlgorithmContainer         m_SortAlgorithms;

        QuadContainer                  m_Quads;
        glm::vec4                      m_BeginColor;
        glm::vec4                      m_EndColor;

        std::future<void>              m_SortFuture;
        std::atomic_bool               m_IsRunning;
        std::atomic_bool               m_IsSorted;
        std::atomic_uint32_t           m_AsyncTasks;
        uint32_t                       m_DelayTime;

        ParticleSystem                 m_ParticleSystem;
        ParticleProps                  m_Particle;
        uint32_t                       m_GenParticleCount;

        ControllableAlgorithmCommand   m_AlgorithmCommand;

    }; // class SortLayer

    template <typename Function, typename... Args>
    std::future<void> SortLayer::AsyncRun(Function&& function, Args&&... args) {
        return std::async(std::launch::async, [
            &,
            function = std::forward<Function>(function),
            ...args  = std::forward<Args>(args)
        ] {
            ++m_AsyncTasks;
            function(std::forward<Args>(args)...);
            --m_AsyncTasks;
        });
    }

    template <SortLayer::ShuffleType ShuffleType>
    void SortLayer::Shuffle() {
        if (m_IsRunning)
            return;

        m_SortFuture = AsyncRun([&] {
            m_IsRunning = true;
            m_IsSorted  = false;

            (*m_ShuffleAlgorithms[ShuffleType])(m_Quads.begin(), m_Quads.end());

            m_IsRunning = false;
        });
    }

    template <SortLayer::SortType SortType>
    void SortLayer::Sort() {
        if (m_IsSorted || m_IsRunning)
            return;

        m_SortFuture = AsyncRun([&] {
            m_IsRunning = true;

            (*m_SortAlgorithms[SortType])(m_Quads.begin(), m_Quads.end());

            m_IsSorted  = true;
            m_IsRunning = false;
        });
    }

} // namespace Sandbox