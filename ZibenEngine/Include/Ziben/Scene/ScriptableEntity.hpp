#pragma once

#include "Entity.hpp"

namespace Ziben {

    class ScriptableEntity {
    public:
        friend class Scene;

    public:
        ScriptableEntity() = default;
        virtual ~ScriptableEntity() = default;

    public:
        template <typename Component>
        [[nodiscard]] inline Component& GetComponent() { return m_Entity.GetComponent<Component>(); }

        template <typename Component>
        [[nodiscard]] inline const Component& GetComponent() const { return m_Entity.GetComponent<Component>(); }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(const TimeStep& ts) {}

    protected:

    private:
        Entity m_Entity;

    }; // class ScriptableEntity

} // namespace Ziben

#include "ScriptableEntity.inl"