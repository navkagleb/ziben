#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace YAML {

   class Emitter;

} // namespace YAML

namespace Ziben {

    class SceneSerializer {
    public:
        explicit SceneSerializer(const Ref<Scene>& context);
        ~SceneSerializer() = default;

    public:
        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);

    private:
        static void SerializeEntity(YAML::Emitter& out, const Entity& entity);

    private:
        Ref<Scene> m_Context;

    }; // class SceneSerializer

} // namespace Ziben