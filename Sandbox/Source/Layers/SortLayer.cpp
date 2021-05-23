#include "SortLayer.hpp"

#include <future>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>

#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application/Application.hpp"
#include "Algorithms/Algorithm.hpp"

namespace Sandbox {

    namespace Internal {

        void Sleep(uintmax_t ms) {
            auto end = clock() + ms * CLOCKS_PER_SEC / 1000;
            while (clock() < end);
        }

    } // namespace Internal

    SortLayer::SortLayer()
        : Ziben::Layer("SortLayer")
        , m_Camera(
            0,
            static_cast<float>(Application::Get().GetWindow().GetWidth()),
            static_cast<float>(Application::Get().GetWindow().GetHeight()),
            0
        )
        , m_QuadCount(50)
        , m_BeginColor(254.0f / 255.0f, 212.0f / 255.0f, 123.0f / 255.0f, 1.0f)
        , m_EndColor(254.0f / 255.0f, 109.0f / 255.0f, 41.0f / 255.0f, 1.0f)
        , m_IsSorted(true)
        , m_DelayTime(1) {}

    SortLayer::~SortLayer() noexcept {
        if (m_SortFuture.valid())
            m_SortFuture.wait();
    }

    void SortLayer::OnAttach() {
        InitQuads();
        UpdateQuads();
    }

    void SortLayer::OnDetach() {

    }

    void SortLayer::OnEvent(Ziben::Event& event) {
        Ziben::EventDispatcher dispatcher(event);

        dispatcher.Dispatch<Ziben::WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
    }

    void SortLayer::OnUpdate(const Ziben::TimeStep& ts) {

    }

    void SortLayer::OnRender() {
//        ZIBEN_INFO("Render");

        Ziben::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Ziben::RenderCommand::Clear();

        Ziben::Renderer2D::BeginScene(m_Camera);
        {
            for (const auto& quad : m_Quads)
                Ziben::Renderer2D::DrawQuad(quad.Position, quad.Size, quad.Color);
        }
        Ziben::Renderer2D::EndScene();
    }

    void SortLayer::OnImGuiRender() {
        ImGui::Begin("Sorting");
        {
            if (ImGui::Button("Shuffle"))
                ShuffleQuads();

            if (ImGui::Button("BubbleSort"))
                BubbleSortQuads();

            if (ImGui::Button("SelectionSort"))
                SelectionSortQuads();

            if (ImGui::Button("InsertionSort"))
                InsertionSortQuads();

            if (ImGui::Button("ShellSort"))
                ShellSortQuads();

            if (ImGui::Button("QuickSort"))
                QuickSortQuads();
        }
        ImGui::End();

        ImGui::Begin("Settings");
        {
            ImGui::Text("Sorted: %s", m_IsSorted ? "True" : "False");
            ImGui::Text("QuadCount: %llu", m_QuadCount);

            ImGui::DragInt("Delay Time (us)", &m_DelayTime, 0.3f, 0, 1000);

            if (ImGui::ColorEdit4("BeginColor", glm::value_ptr(m_BeginColor)))
                UpdateQuads();

            if (ImGui::ColorEdit4("EndColor", glm::value_ptr(m_EndColor)))
                UpdateQuads();

            if (m_IsSorted && ImGui::DragInt("QuadCount", reinterpret_cast<int*>(&m_QuadCount), 0.3f, 2, 400))
                UpdateQuads();
        }
        ImGui::End();
    }

    bool SortLayer::OnWindowResized(Ziben::WindowResizedEvent& event) {
        m_Camera.SetProjection(
            0,
            static_cast<float>(Application::Get().GetWindow().GetWidth()),
            static_cast<float>(Application::Get().GetWindow().GetHeight()),
            0
        );

        UpdateQuads();

        return true;
    }

    void SortLayer::Sleep() const {


        std::cout << std::endl;
    }

    void SortLayer::InitQuads() {
        std::size_t prevCount = m_Quads.size();

        m_Quads.resize(m_QuadCount);

        for (std::size_t i = prevCount; i < m_Quads.size(); ++i)
            m_Quads[i].Index = i;
    }

    void SortLayer::ShuffleQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            m_IsSorted = false;

            Algorithm::Shuffle(m_Quads.begin(), m_Quads.end(), [&](auto index) {
                Internal::Sleep(10);
                UpdateQuads();
                return Ziben::Random::GetFromRange<decltype(index)>(0, index);
            });
        });
    }

    void SortLayer::UpdateQuads() {
        if (m_QuadCount != m_Quads.size())
           InitQuads();

        auto width   = static_cast<float>(Application::Get().GetWindow().GetWidth());
        auto height  = static_cast<float>(Application::Get().GetWindow().GetHeight());

        float scaleX = width  / static_cast<float>(m_QuadCount);
        float scaleY = height / static_cast<float>(m_QuadCount);

        for (std::size_t i = 0; i < m_Quads.size(); ++i) {
            m_Quads[i].Size     = { scaleX, scaleY * static_cast<float>(m_Quads[i].Index + 1) };
            m_Quads[i].Position = { scaleX * static_cast<float>(i + 1) - m_Quads[i].Size.x / 2.0f, height - m_Quads[i].Size.y / 2.0f };
            m_Quads[i].Color    = glm::lerp(m_EndColor, m_BeginColor, m_Quads[i].Size.y / height);
        }
    }

    void SortLayer::SwapQuads(Quad& lhs, Quad& rhs) {
        std::swap(lhs.Index, rhs.Index);
        UpdateQuads();
    }

    void SortLayer::BubbleSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            Algorithm::BubbleSort(m_Quads.begin(), m_Quads.end(), [&](const auto& lhs, const auto& rhs) {
                Internal::Sleep(5);
                UpdateQuads();

                return lhs.Index < rhs.Index;
            });

            m_IsSorted = true;
        });
    }

    void SortLayer::SelectionSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            Algorithm::SelectionSort(m_Quads.begin(), m_Quads.end(), [&](const auto& lhs, const auto& rhs) {
                Internal::Sleep(1);
                UpdateQuads();

                return lhs.Index < rhs.Index;
            });

            UpdateQuads();

            m_IsSorted = true;
        });
    }

    void SortLayer::InsertionSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            Algorithm::InsertionSort(m_Quads.begin(), m_Quads.end(), [&](const auto& lhs, const auto& rhs) {
                Internal::Sleep(10);
                UpdateQuads();

                return lhs.Index < rhs.Index;
            });

            m_IsSorted = true;
        });
    }

    void SortLayer::ShellSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            Algorithm::ShellSort(m_Quads.begin(), m_Quads.end(), [&](const auto& lhs, const auto& rhs) {
                Internal::Sleep(10);
                UpdateQuads();

                return lhs.Index < rhs.Index;
            });

            m_IsSorted = true;
        });
    }

    void SortLayer::QuickSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            Algorithm::QuickSort(m_Quads.begin(), m_Quads.end(), [&](const auto& lhs, const auto& rhs) {
                Internal::Sleep(10);
                UpdateQuads();

                return lhs.Index < rhs.Index;
            });

            m_IsSorted = true;
        });
    }

} // namespace Sandbox