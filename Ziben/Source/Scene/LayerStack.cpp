#include "LayerStack.hpp"

#include "Layer.hpp"

namespace Ziben {

    LayerStack::LayerStack()
        : m_Index(0) {}

    LayerStack::~LayerStack() {
        for (Layer* layer : m_Layers) {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer) {
        m_Layers.emplace(m_Layers.begin() + m_Index++, layer);
    }

    void LayerStack::PushOverlay(Layer* layer) {
        m_Layers.emplace_back(layer);
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_Index, layer);

        if (it != m_Layers.begin() + m_Index) {
            layer->OnDetach();
            m_Layers.erase(it);
            --m_Index;
        }
    }

    void LayerStack::PopOverlay(Layer* layer) {
        auto it = std::find(m_Layers.begin() + m_Index, m_Layers.end(), layer);

        if (it != m_Layers.end()) {
            layer->OnDetach();
            m_Layers.erase(it);
        }
    }

} // namespace Ziben