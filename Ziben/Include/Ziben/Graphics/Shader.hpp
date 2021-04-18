#pragma once

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

    class Shader {

    }; // class Shader

} // namespace Ziben