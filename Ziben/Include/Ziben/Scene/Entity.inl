namespace Ziben {

    template <typename Component>
    bool Entity::HasComponent() const {
        return m_Scene->m_Registry.all_of<Component>(m_Handle);
    }

    template <typename Component>
    Component& Entity::GetComponent() {
        assert(HasComponent<Component>());
        return m_Scene->m_Registry.get<Component>(m_Handle);
    }

    template <typename Component>
    const Component& Entity::GetComponent() const {
        assert(HasComponent<Component>());
        return m_Scene->m_Registry.get<Component>(m_Handle);
    }

    template <typename Component, typename... Args>
    Component& Entity::GetOrPushComponent(Args&&... args) {
        return m_Scene->m_Registry.get_or_emplace<Component>(m_Handle, std::forward<Args>(args)...);
    }

    template <typename Component, typename... Args>
    Component& Entity::PushComponent(Args&&... args) {
        assert(!HasComponent<Component>());
        return m_Scene->m_Registry.emplace<Component>(m_Handle, std::forward<Args>(args)...);
    }

    template <typename Component>
    void Entity::PopComponent() {
        assert(HasComponent<Component>());
        m_Scene->m_Registry.remove<Component>(m_Handle);
    }

} // namespace Ziben