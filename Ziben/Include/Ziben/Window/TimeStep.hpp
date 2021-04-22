#pragma once

namespace Ziben {

    class TimeStep {
    public:
        explicit TimeStep();

        [[nodiscard]] inline float GetSeconds() const { return m_TimeStep; }
        [[nodiscard]] inline float GetMilliseconds() const { return m_TimeStep * 1000.0f; }

        void Update(float currentFrameTime);

    public:
        explicit inline operator float() const { return m_TimeStep; }

    private:
        float m_TimeStep;
        float m_LastTime;
        float m_Elapsed;

    }; // class TimeStep

} // namespace Ziben