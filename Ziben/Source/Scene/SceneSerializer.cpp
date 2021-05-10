#include "SceneSerializer.hpp"

#include <yaml-cpp/yaml.h>

#include "Component.hpp"

namespace YAML {

    template <>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& vec3) {
            Node node;

            node.push_back(vec3.x);
            node.push_back(vec3.y);
            node.push_back(vec3.z);

            return node;
        }

        static bool decode(const Node& node, glm::vec3& vec3) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            vec3.x = node[0].as<float>();
            vec3.y = node[1].as<float>();
            vec3.z = node[2].as<float>();

            return true;
        }
    };

    template <>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& vec4) {
            Node node;

            node.push_back(vec4.x);
            node.push_back(vec4.y);
            node.push_back(vec4.z);
            node.push_back(vec4.w);

            return node;
        }

        static bool decode(const Node& node, glm::vec4& vec4) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            vec4.x = node[0].as<float>();
            vec4.y = node[1].as<float>();
            vec4.z = node[2].as<float>();
            vec4.w = node[3].as<float>();

            return true;
        }
    };

} // namespace YAML

namespace Ziben {

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& vec3) {
        out << YAML::Flow;
        out << YAML::BeginSeq;
        {
            out << vec3.x;
            out << vec3.y;
            out << vec3.z;
        }
        out << YAML::EndSeq;

        return out;
    }

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& vec4) {
        out << YAML::Flow;
        out << YAML::BeginSeq;
        {
            out << vec4.x;
            out << vec4.y;
            out << vec4.z;
            out << vec4.w;
        }
        out << YAML::EndSeq;

        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& context)
        : m_Context(context) {}

    void SceneSerializer::Serialize(const std::string& filepath) {
        YAML::Emitter out;

        out << YAML::BeginMap;
        {
            out << YAML::Key   << "Scene";
            out << YAML::Value << "Untitled";

            out << YAML::Key   << "Entities";
            out << YAML::Value << YAML::BeginSeq;
            {
                m_Context->m_Registry.each([&](entt::entity handle) {
                    Entity entity(handle, m_Context.get());

                    if (!entity)
                        return;

                    SerializeEntity(out, entity);
                });
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;

        std::ofstream outfile(filepath);
        outfile << out.c_str();
        outfile.close();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath) {
        // Not implemented
        assert(false);
    }

    bool SceneSerializer::Deserialize(const std::string& filepath) {
        std::ifstream     infile(filepath);
        std::stringstream ss;

        assert(infile);

        ss << infile.rdbuf();

        YAML::Node data = YAML::Load(ss.str());

        if (!data["Scene"])
            return false;

        auto sceneName = data["Scene"].as<std::string>();
        ZIBEN_CORE_INFO("Deserializing scene {0}", sceneName);

        YAML::Node entities = data["Entities"];

        if (entities) {
            for (const auto& entity : entities) {
                auto        uuid = entity["Entity"].as<uint64_t>();
                std::string name;

                if (auto component = entity["TagComponent"])
                    name = component["Tag"].as<std::string>();

                ZIBEN_CORE_INFO("Deserializing entity with ID = {0}, name = {1}", uuid, name);
                Entity deserializedEntity = m_Context->CreateEntity(name);

                if (auto serializedComponent = entity["TransformComponent"]) {
                    auto& entityComponent = deserializedEntity.GetOrPushComponent<TransformComponent>();

                    entityComponent.SetTranslation(serializedComponent["Translation"].as<glm::vec3>());
                    entityComponent.SetRotation(serializedComponent["Rotation"].as<glm::vec3>());
                    entityComponent.SetScale(serializedComponent["Scale"].as<glm::vec3>());
                }

                if (auto serializedComponent = entity["CameraComponent"]) {
                    auto& component                   = deserializedEntity.GetOrPushComponent<CameraComponent>();
                    auto  serializedCamera            = serializedComponent["Camera"];
                    auto  serializedPerspectiveProps  = serializedCamera["PerspectiveProps"];
                    auto  serializedOrthographicProps = serializedCamera["OrthographicProps"];

                    component.Camera.SetPerspective({
                        serializedPerspectiveProps["Fov"].as<float>(),
                        serializedPerspectiveProps["Near"].as<float>(),
                        serializedPerspectiveProps["Far"].as<float>()
                    });

                    component.Camera.SetOrthographic({
                        serializedOrthographicProps["Size"].as<float>(),
                        serializedOrthographicProps["Near"].as<float>(),
                        serializedOrthographicProps["Far"].as<float>()
                    });

                    component.Camera.SetProjectionType(
                        static_cast<SceneCamera::ProjectionType>(serializedCamera["ProjectionType"].as<int>())
                    );

                    component.IsPrimary           = serializedComponent["IsPrimary"].as<bool>();
                    component.HasFixedAspectRatio = serializedComponent["HasFixedAspectRatio"].as<bool>();
                }

                if (auto serializedComponent = entity["SpriteRendererComponent"]) {
                    auto& component = deserializedEntity.GetOrPushComponent<SpriteRendererComponent>();

                    component.Color = serializedComponent["Color"].as<glm::vec4>();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
        // Not implemented
        assert(false);
        return false;
    }

    void SceneSerializer::SerializeEntity(YAML::Emitter& out, const Entity& entity) {
        out << YAML::BeginMap;
        {
            out << YAML::Key   << "Entity";
            out << YAML::Value << "12391723912837";

            if (entity.HasComponent<TagComponent>()) {
                out << YAML::Key << "TagComponent";
                out << YAML::BeginMap;
                {
                    auto& component = entity.GetComponent<TagComponent>();

                    out << YAML::Key   << "Tag";
                    out << YAML::Value << component.Tag;
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<TransformComponent>()) {
                out << YAML::Key << "TransformComponent";
                out << YAML::BeginMap;
                {
                    auto& component = entity.GetComponent<TransformComponent>();

                    out << YAML::Key   << "Translation";
                    out << YAML::Value << component.GetTranslation();

                    out << YAML::Key   << "Rotation";
                    out << YAML::Value << component.GetRotation();

                    out << YAML::Key   << "Scale";
                    out << YAML::Value << component.GetScale();
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<CameraComponent>()) {
                out << YAML::Key << "CameraComponent";
                out << YAML::BeginMap;
                {
                    auto& component = entity.GetComponent<CameraComponent>();

                    out << YAML::Key << "Camera";
                    out << YAML::BeginMap;
                    {
                        out << YAML::Key << "ProjectionType";
                        out << YAML::Value << static_cast<int>(component.Camera.GetProjectionType());

                        out << YAML::Key << "PerspectiveProps";
                        out << YAML::BeginMap;
                        {
                            out << YAML::Key   << "Fov";
                            out << YAML::Value << component.Camera.GetPerspectiveProps().Fov;

                            out << YAML::Key   << "Near";
                            out << YAML::Value << component.Camera.GetPerspectiveProps().Near;

                            out << YAML::Key   << "Far";
                            out << YAML::Value << component.Camera.GetPerspectiveProps().Far;
                        }
                        out << YAML::EndMap;

                        out << YAML::Key << "OrthographicProps";
                        out << YAML::BeginMap;
                        {
                            out << YAML::Key   << "Size";
                            out << YAML::Value << component.Camera.GetOrthographicProps().Size;

                            out << YAML::Key   << "Near";
                            out << YAML::Value << component.Camera.GetOrthographicProps().Near;

                            out << YAML::Key   << "Far";
                            out << YAML::Value << component.Camera.GetOrthographicProps().Far;
                        }
                        out << YAML::EndMap;
                    }
                    out << YAML::EndMap;

                    out << YAML::Key   << "IsPrimary";
                    out << YAML::Value << component.IsPrimary;

                    out << YAML::Key   << "HasFixedAspectRatio";
                    out << YAML::Value << component.HasFixedAspectRatio;
                }
                out << YAML::EndMap;
            }

            if (entity.HasComponent<SpriteRendererComponent>()) {
                out << YAML::Key << "SpriteRendererComponent";
                out << YAML::BeginMap;
                {
                    auto& component = entity.GetComponent<SpriteRendererComponent>();

                    out << YAML::Key   << "Color";
                    out << YAML::Value << component.Color;
                }
                out << YAML::EndMap;
            }
        }
        out << YAML::EndMap;
    }

} // namespace Ziben