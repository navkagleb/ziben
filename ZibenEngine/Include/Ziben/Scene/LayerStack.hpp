#pragma once

namespace Ziben {

    class Layer;

    class LayerStack {
    public:
        using LayerContainer            = std::vector<Layer*>;
        using LayerIterator             = LayerContainer::iterator;
        using LayerConstIterator        = LayerContainer::const_iterator;
        using LayerReverseIterator      = LayerContainer::reverse_iterator;
        using LayerConstReverseIterator = LayerContainer::const_reverse_iterator;

    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        [[nodiscard]] inline LayerIterator Begin() { return m_Layers.begin(); }
        [[nodiscard]] inline LayerIterator End() { return m_Layers.end(); }

        [[nodiscard]] inline LayerConstIterator Begin() const { return m_Layers.begin(); }
        [[nodiscard]] inline LayerConstIterator End() const { return m_Layers.end(); }

        [[nodiscard]] inline LayerReverseIterator ReverseBegin() { return m_Layers.rbegin(); }
        [[nodiscard]] inline LayerReverseIterator ReverseEnd() { return m_Layers.rend(); }

        [[nodiscard]] inline LayerConstReverseIterator ReverseBegin() const { return m_Layers.rbegin(); }
        [[nodiscard]] inline LayerConstReverseIterator ReverseEnd() const { return m_Layers.rend(); }

    private:
        LayerContainer                  m_Layers;
        LayerContainer::difference_type m_Index;

    }; // class LayerStack

    // For range based for!
    inline auto begin(LayerStack& layerStack) { return layerStack.Begin(); }
    inline auto end(LayerStack& layerStack) { return layerStack.End(); }

    inline auto begin(const LayerStack& layerStack) { return layerStack.Begin(); }
    inline auto end(const LayerStack& layerStack) { return layerStack.End(); }

    inline auto rbegin(LayerStack& layerStack) { return layerStack.ReverseBegin(); }
    inline auto rend(LayerStack& layerStack) { return layerStack.ReverseEnd(); }

    inline auto rbegin(const LayerStack& layerStack) { return layerStack.ReverseBegin(); }
    inline auto rend(const LayerStack& layerStack) { return layerStack.ReverseEnd(); }

} // namespace Ziben