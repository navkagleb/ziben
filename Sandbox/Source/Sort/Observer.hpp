#pragma once

namespace Sandbox {

    class IObserver {
    public:
        IObserver() = default;
        virtual ~IObserver() = default;

    public:
        virtual void OnNotify(const void* data) = 0;

    }; // class IObserver

    template <typename T>
    concept ObserverConcept = std::is_base_of_v<IObserver, T>;

    class Subject {
    public:
        Subject() = default;
        virtual ~Subject() = default;

    public:
        void RegisterObserver(IObserver* observer);
        void UnregisterObserver(IObserver* observer);
        void NotifyObservers(const void* eventData);

    private:
        std::vector<IObserver*> m_Observers;

    }; // class Subject

} // namespace Sandbox