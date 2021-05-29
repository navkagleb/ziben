#include "Observer.hpp"

namespace Sandbox {

    void Subject::RegisterObserver(IObserver* observer) {
        auto observerIt = std::find(m_Observers.begin(), m_Observers.end(), observer);

        if (observerIt == m_Observers.end())
            m_Observers.push_back(observer);
    }

    void Subject::UnregisterObserver(IObserver* observer) {
        auto observerIt = std::find(m_Observers.begin(), m_Observers.end(), observer);

        if (observerIt != m_Observers.end())
            m_Observers.erase(observerIt);

        delete observer;
    }

    void Subject::NotifyObservers(const void* eventData) {
        for (auto& observer : m_Observers)
            observer->OnNotify(eventData);
    }

} // namespace Sandbox