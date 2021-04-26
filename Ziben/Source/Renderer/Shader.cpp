#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Ziben {

    Ref<Shader> Shader::Create() {
        return CreateRef<Shader>();
    }

    void Shader::Bind(const Ref<Shader>& shader) {
        if (!shader->m_IsLinked)
            shader->Link();

        glUseProgram(shader->m_Handle);
    }

    void Shader::Unbind() {
        glUseProgram(0);
    }

    Shader::Shader()
        : m_Handle(0)
        , m_IsLinked(false) {}

    Shader::~Shader() {
        if (m_Handle == 0)
            return;

        int shaderCount = 0;
        glGetProgramiv(m_Handle, GL_ATTACHED_SHADERS, &shaderCount);

        std::vector<HandleType> shaderHandles(shaderCount);
        glGetAttachedShaders(m_Handle, shaderCount, nullptr, shaderHandles.data());

        for (HandleType shaderHandle : shaderHandles)
            glDeleteShader(shaderHandle);

        glDeleteProgram(m_Handle);
    }

    void Shader::Compile(const std::string& filepath) {
        namespace fs = std::filesystem;

        static std::map<std::string, ShaderType> types = {
            { ".vs",   ShaderType::Vertex         },
            { ".vert", ShaderType::Vertex         },
            { ".fs",   ShaderType::Fragment       },
            { ".frag", ShaderType::Fragment       },
            { ".gs",   ShaderType::Geometry       },
            { ".geom", ShaderType::Geometry       },
            { ".tcs",  ShaderType::TessControl    },
            { ".tes",  ShaderType::TessEvaluation },
            { ".cs",   ShaderType::Compute        }
        };

        if (!fs::exists(filepath))
            throw std::invalid_argument("No file by provided filename: " + filepath);

        auto extension = fs::path(filepath).extension().string();

        if (!types.contains(extension))
            throw std::invalid_argument("Unrecognized extension: " + extension);

        Compile(filepath, types[extension]);
    }

    void Shader::Compile(const std::string& filepath, ShaderType type) {
        if (m_Handle == 0) {
            m_Handle = glCreateProgram();

            if (m_Handle == 0)
                throw std::runtime_error("Unable to create shader program!");
        }

        HandleType shaderHandle = glCreateShader(static_cast<GLenum>(type));

        if (shaderHandle == 0)
            throw std::runtime_error("Error creating shader!");

        std::ifstream shaderCode(filepath);

        std::ifstream     infile(filepath);
        std::stringstream result;

        result << infile.rdbuf();

        infile.close();

        std::string shaderCodeString = result.str();
        const char* shaderCodeCString = shaderCodeString.c_str();

        glShaderSource(shaderHandle, 1, &shaderCodeCString, nullptr);
        glCompileShader(shaderHandle);

        int status;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            int logLength;
            glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);

            std::string log(logLength, '\0');

            if (logLength > 0) {
                int written;
                glGetShaderInfoLog(shaderHandle, logLength, &written, log.data());
            }

            if (!log.empty())
                log += ": shader compilation failed!";
            else
                log = "Shader compilation failed!";

            glDeleteShader(shaderHandle);

            throw std::runtime_error(log);
        }

        glAttachShader(m_Handle, shaderHandle);
    }

    void Shader::Link() const {
        glLinkProgram(m_Handle);

        int status;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);

        if (status == GL_TRUE) {
            glValidateProgram(m_Handle);
            m_IsLinked = true;

            return;
        }

        int logLength;
        glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0) {
            std::string log(logLength, '\0');

            int written;
            glGetProgramInfoLog(m_Handle, logLength, &written, log.data());

            throw std::runtime_error(log);
        }
    }

    void Shader::BindAttribLocation(uint32_t location, const std::string& name) const {
        if (!m_IsLinked)
            glBindAttribLocation(m_Handle, location, name.c_str());
    }

    void Shader::BindFragDataLocation(uint32_t location, const std::string& name) const {
        if (!m_IsLinked)
            glBindFragDataLocation(m_Handle, location, name.c_str());
    }

    void Shader::SetUniform(const std::string& name, bool value) {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform(const std::string& name, int value) {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniform(const std::string& name, float value) {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniform(const std::string& name, float x, float y, float z) {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    void Shader::SetUniform(const std::string& name, const glm::vec3& vec3) {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec3));
    }

    void Shader::SetUniform(const std::string& name, const glm::vec4& vec4) {
        glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec4));
    }

    void Shader::SetUniform(const std::string& name, const glm::mat3& mat3) {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat3));
    }

    void Shader::SetUniform(const std::string& name, const glm::mat4& mat4) {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
    }
    
    int Shader::GetUniformLocation(const std::string& name) {
        if (!m_UniformLocations.contains(name)) {
            int location = glGetUniformLocation(m_Handle, name.c_str());

            if (location >= 0)
                return m_UniformLocations[name] = location;
        }

        return m_UniformLocations[name];
    }

} // namespace Ziben