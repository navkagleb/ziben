namespace Ziben {

    template <typename T>
    void NativeScriptComponent::Bind() {
        m_InstantiateFunction     = [&] { m_Instance = new T(); };
        m_DestroyInstanceFunction = [&] { delete (T*)m_Instance; };

        m_OnCreateFunction  = [](ScriptableEntity* instance) { return ((T*)instance)->OnCreate(); };
        m_OnDestroyFunction = [](ScriptableEntity* instance) { return ((T*)instance)->OnDestroy(); };
        m_OnUpdateFunction  = [](ScriptableEntity* instance, const TimeStep& ts) { return ((T*)instance)->OnUpdate(ts); };
    }

} // namespace Ziben