#pragma once

#include <future>

#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCamera.hpp>

namespace Sandbox {

    class SortLayer : public Ziben::Layer {
    public:
        SortLayer();
        ~SortLayer() noexcept override;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Ziben::Event& event) override;
        void OnUpdate(const Ziben::TimeStep& ts) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        struct Quad {
            std::size_t Index    = 0;
            glm::vec2   Position = glm::vec2(0.0f);
            glm::vec2   Size     = glm::vec2(1.0f);
            glm::vec4   Color    = glm::vec4(1.0f);
        };

    private:
        bool OnWindowResized(Ziben::WindowResizedEvent& event);

        void Sleep() const;

        void InitQuads();
        void ShuffleQuads();
        void UpdateQuads();
        void SwapQuads(Quad& lhs, Quad& rhs);

        void BubbleSortQuads();
        void SelectionSortQuads();
        void InsertionSortQuads();
        void ShellSortQuads();

    private:
        Ziben::OrthographicCamera m_Camera;

        std::size_t               m_QuadCount;
        std::vector<Quad>         m_Quads;
        glm::vec4                 m_BeginColor;
        glm::vec4                 m_EndColor;

        bool                      m_IsSorted;

        std::future<void>         m_SortFuture;
        int                       m_DelayTime;

    }; // class SortLayer

} // namespace Sandbox