#include "SortLayer.hpp"

#include <future>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>

#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application/Application.hpp"
#include "Algorithms/Algorithms.hpp"

namespace Sandbox {

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
        }
        ImGui::End();

        ImGui::Begin("Settings");
        {
            ImGui::Text("Sorted: %s", m_IsSorted ? "True" : "False");
            ImGui::Text("QuadCount: %llu", m_QuadCount);

            ImGui::DragInt("Delay Time (ms)", &m_DelayTime, 0.1f, 0, 200);

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
        std::this_thread::sleep_for(std::chrono::milliseconds(m_DelayTime));
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

            for (std::size_t i = m_Quads.size() - 1; i > 0; --i) {
                SwapQuads(m_Quads[i], m_Quads[Ziben::Random::GetRef().Get<std::size_t>(0, i)]);
                Sleep();
            }
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
            bool isSwapped;

            for (int i = 0; i < m_QuadCount - 1; ++i) {
                isSwapped = false;

                for (int j = 0; j < m_QuadCount - i - 1; ++j) {
                    if (m_Quads[j].Index > m_Quads[j + 1].Index) {
                        isSwapped = true;

                        SwapQuads(m_Quads[j], m_Quads[j + 1]);
                        Sleep();
                    }
                }

                if (!isSwapped)
                    break;
            }

            m_IsSorted = true;
        });
    }

    void SortLayer::SelectionSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            std::size_t minIndex;

            for (int i = 0; i < m_QuadCount - 1; ++i) {
                minIndex = i;

                for (int j = i; j < m_QuadCount; ++j)
                    if (m_Quads[j].Index < m_Quads[minIndex].Index)
                        minIndex = j;

                SwapQuads(m_Quads[i], m_Quads[minIndex]);
                Sleep();
            }

            m_IsSorted = true;
        });
    }

    void SortLayer::InsertionSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            for (int i = 1; i < m_QuadCount; ++i) {
                for (int j = i; j > 0 && m_Quads[j].Index < m_Quads[j - 1].Index; --j) {
                    SwapQuads(m_Quads[j], m_Quads[j - 1]);
                    Sleep();
                }
            }

            m_IsSorted = true;
        });
    }

    void SortLayer::ShellSortQuads() {
        m_SortFuture = std::async(std::launch::async, [&] {
            int h = 1;

            while (h < m_QuadCount / 3)
                h = 3 * h + 1;

            while (h >= 1) {
                for (int i = h; i < m_QuadCount; ++i) {
                    for (int j = i; j >= h && m_Quads[j].Index < m_Quads[j - h].Index; j -= h) {
                        SwapQuads(m_Quads[j], m_Quads[j - h]);
                        Sleep();
                    }
                }

                h = h / 3;
            }

            m_IsSorted = true;
        });
    }

} // namespace Sandbox