namespace Ziben {

    template <typename T>
    void NativeScriptComponent::Bind() {
        m_InstantiateScript = [] { return static_cast<ScriptableEntity*>(new T()); };

        m_DestroyScript = [](NativeScriptComponent* component) {
            delete component->m_Instance;
            component->m_Instance = nullptr;
        };
    }

} // namespace Ziben