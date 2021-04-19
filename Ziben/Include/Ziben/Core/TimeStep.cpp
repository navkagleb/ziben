#include "TimeStep.hpp"

namespace Ziben {

    TimeStep::TimeStep()
        : m_TimeStep(0.0f)
        , m_LastTime(0.0f)
        , m_Elapsed(0.0f) {}

    void TimeStep::Update(float currentFrameTime) {
        m_TimeStep  = currentFrameTime - m_LastTime;
        m_LastTime  = currentFrameTime;
        m_Elapsed  += m_TimeStep;
    }

} // namespace Ziben