#pragma once

#include <Ziben/Window/TimeStep.hpp>
#include <Ziben/Renderer/OrthographicCamera.hpp>

struct ParticleProps {
    glm::vec2 Position          = glm::vec2(0.0f);
    glm::vec2 Velocity          = glm::vec2(0.0f);
    glm::vec2 VelocityVariation = glm::vec2(0.0f);
    glm::vec4 ColorBegin        = glm::vec4(0.0f);
    glm::vec4 ColorEnd          = glm::vec4(0.0f);

    float     SizeBegin         = 0.0f;
    float     SizeEnd           = 0.0f;
    float     SizeVariation     = 0.0f;
    float     LifeTime          = 1.0f;
};

class ParticleSystem {
public:
    explicit ParticleSystem(uint32_t maxParticleCount = 1000);
    ~ParticleSystem() = default;

    void OnUpdate(const Ziben::TimeStep& ts);
    void OnRender(const Ziben::OrthographicCamera& camera);

    void Push(const ParticleProps& props);

private:
    struct Particle {
        glm::vec2 Position      = glm::vec2(0.0f);
        glm::vec2 Velocity      = glm::vec2(0.0f);
        glm::vec4 ColorBegin    = glm::vec4(0.0f);
        glm::vec4 ColorEnd      = glm::vec4(0.0f);

        float     Rotation      = 0.0f;
        float     SizeBegin     = 0.0f;
        float     SizeEnd       = 0.0f;
        float     LifeTime      = 1.0f;
        float     LifeRemaining = 1.0f;

        bool      Active        = false;
    };

private:
    std::vector<Particle> m_ParticlePool;
    uint32_t              m_PoolIndex;

}; // class ParticleSystem