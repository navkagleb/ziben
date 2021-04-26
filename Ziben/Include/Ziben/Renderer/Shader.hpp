#pragma once

#include <glm/glm.hpp>

#include "GraphicsCore.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace Ziben {

    namespace ShaderData {

        enum class Type : uint8_t {
            None = 0,
            Int, Int2, Int3, Int4,
            Float, Float2, Float3, Float4,
            Mat3, Mat4
        };

        static inline constexpr int GetSize(Type type) {
            switch (type) {
                case Type::Int:    return sizeof(int) * 1;
                case Type::Int2:   return sizeof(int) * 2;
                case Type::Int3:   return sizeof(int) * 3;
                case Type::Int4:   return sizeof(int) * 4;
                case Type::Float:  return sizeof(float) * 1;
                case Type::Float2: return sizeof(float) * 2;
                case Type::Float3: return sizeof(float) * 3;
                case Type::Float4: return sizeof(float) * 4;
                case Type::Mat3:   return sizeof(float) * 3 * 3;
                case Type::Mat4:   return sizeof(float) * 4 * 4;
                default:           break;
            }

            throw std::invalid_argument("Not supported type");
        }

        static inline constexpr int GetCount(Type type) {
            switch (type) {
                case Type::Int:    return 1;
                case Type::Int2:   return 2;
                case Type::Int3:   return 3;
                case Type::Int4:   return 4;
                case Type::Float:  return 1;
                case Type::Float2: return 2;
                case Type::Float3: return 3;
                case Type::Float4: return 4;
                case Type::Mat3:   return 3 * 3;
                case Type::Mat4:   return 4 * 4;
                default:           break;
            }

            throw std::invalid_argument("Not supported type");
        }

        static inline constexpr GLenum ToNativeType(Type type) {
            switch (type) {
                case Type::Int:
                case Type::Int2:
                case Type::Int3:
                case Type::Int4:   return GL_INT;
                case Type::Float:
                case Type::Float2:
                case Type::Float3:
                case Type::Float4:
                case Type::Mat3:
                case Type::Mat4:   return GL_FLOAT;
                default:           break;
            }

            throw std::invalid_argument("Not supported type");
        }

    } // namespace ShaderData

    enum class ShaderType : uint16_t {
        Vertex         = GL_VERTEX_SHADER,
        Fragment       = GL_FRAGMENT_SHADER,
        Geometry       = GL_GEOMETRY_SHADER,
        TessControl    = GL_TESS_CONTROL_SHADER,
        TessEvaluation = GL_TESS_EVALUATION_SHADER,
        Compute        = GL_COMPUTE_SHADER
    };

    class Shader {
    public:
        static Ref<Shader> Create();

        static void Bind(const Ref<Shader>& shader);
        static void Unbind();

    public:
        Shader();
        ~Shader();

        void Compile(const std::string& filepath);
        void Compile(const std::string& filepath, ShaderType type);

        void BindAttribLocation(uint32_t location, const std::string& name) const;
        void BindFragDataLocation(uint32_t location, const std::string& name) const;

        void SetUniform(const std::string& name, bool value);
        void SetUniform(const std::string& name, int value);
        void SetUniform(const std::string& name, float value);
        void SetUniform(const std::string& name, float x, float y, float z);
        void SetUniform(const std::string& name, const glm::vec3& vec3);
        void SetUniform(const std::string& name, const glm::vec4& vec4);
        void SetUniform(const std::string& name, const glm::mat3& mat3);
        void SetUniform(const std::string& name, const glm::mat4& mat4);

    private:
        int GetUniformLocation(const std::string& name);

        void Link() const;

    private:
        HandleType                 m_Handle;
        mutable bool               m_IsLinked;
        std::map<std::string, int> m_UniformLocations;

    }; // class Shader

} // namespace Ziben
