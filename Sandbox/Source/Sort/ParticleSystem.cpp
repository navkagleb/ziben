#include "ParticleSystem.hpp"

#include <glm/gtx/compatibility.hpp>

#include <Ziben/Renderer/Renderer2D.hpp>
#include <Ziben/Utility/Random.hpp>

namespace Sandbox {

    ParticleSystem::ParticleSystem(uint32_t maxParticleCount)
        : m_ParticlePool(maxParticleCount)
        , m_PoolIndex(maxParticleCount - 1) {}

    void ParticleSystem::SetMaxParticleCount(uint32_t maxParticleCount) {
        m_ParticlePool.resize(maxParticleCount);

        m_PoolIndex = maxParticleCount - 1;
    }

    void ParticleSystem::OnUpdate(const Ziben::TimeStep& ts) {
        ZIBEN_PROFILE_FUNCTION();

        for (auto& particle : m_ParticlePool) {
            if (!particle.Active)
                continue;

            if (particle.LifeRemaining < 0.0f) {
                particle.Active = false;
                continue;
            }

            particle.LifeRemaining -= (float)ts;
            particle.Position      += particle.Velocity * (float)ts;
            particle.Rotation      += Ziben::Random::GetFromRange(0.0f, 90.0f) * (float)ts;
        }
    }

    void ParticleSystem::OnRender(const Ziben::OrthographicCamera& camera) {
        ZIBEN_PROFILE_FUNCTION();

        Ziben::Renderer2D::BeginScene(camera);
        {
            for (const auto& particle : m_ParticlePool) {
                if (!particle.Active)
                    continue;

                float     life   = std::max(0.0f, particle.LifeRemaining / particle.LifeTime);
                float     size   = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
                float     zIndex = glm::lerp(0.0f, 1.0f, life);
                glm::vec4 color  = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

                Ziben::Renderer2D::DrawRotatedQuad(
                    { particle.Position.x, particle.Position.y, zIndex },
                    { size, size },
                    glm::radians(particle.Rotation),
                    color
                );
            }
        }
        Ziben::Renderer2D::EndScene();
    }

    void ParticleSystem::Push(const ParticleProps& props) {
        Particle& particle     = m_ParticlePool[m_PoolIndex];

        particle.Active        = true;
        particle.Position      = props.Position;
        particle.Rotation      = Ziben::Random::GetFromRange(0.0f, glm::pi<float>());

        // Velocity
        particle.Velocity      = props.Velocity;
        particle.Velocity.x   += props.VelocityVariation.x * Ziben::Random::GetFromRange(-2.0f, 2.0f);
        particle.Velocity.y   += props.VelocityVariation.y * Ziben::Random::GetFromRange(-2.0f, 2.0f);

        // Color
        particle.ColorBegin    = props.ColorBegin;
        particle.ColorEnd      = props.ColorEnd;

        particle.LifeTime      = props.LifeTime;
        particle.LifeRemaining = props.LifeTime;
        particle.SizeBegin     = props.SizeBegin + props.SizeVariation * Ziben::Random::GetFromRange(-1.0f, 1.0f);
        particle.SizeEnd       = props.SizeEnd;

        --m_PoolIndex;

        if (m_PoolIndex >= m_ParticlePool.size())
            m_PoolIndex = m_ParticlePool.size() - 1;
    }

} // namespace Sandbox