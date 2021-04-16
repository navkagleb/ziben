#pragma once

#include "NonCopyable.hpp"
#include "NonMoveable.hpp"

namespace Ziben {

    template <typename T>
    class Singleton {
    public:
        template <typename... Args>
        static T& Create(Args&&... args);

        static void Destroy();
        static inline T& Get();
        static inline T* GetPointer();

        NON_COPYABLE(Singleton);
        NON_MOVEABLE(Singleton);

    private:
        Singleton() = default;
        ~Singleton() = default;

        static inline T* s_Instance = nullptr;

    }; // class Singleton

    template <typename T>
    template <typename... Args>
    T& Singleton<T>::Create(Args&&... args) {
        if (s_Instance) {
            std::stringstream ss;
            ss << "Ziben::Singleton<" << typeid(T).name() << ">::Create: s_Instance is not nullptr!";
            throw std::runtime_error(ss.str());
        }

        return *(s_Instance = new T(std::forward<Args>(args)...));
    }

    template <typename T>
    void Singleton<T>::Destroy() {
        delete s_Instance;
        s_Instance = nullptr;
    }

    template <typename T>
    inline T& Singleton<T>::Get() {
        if (!s_Instance) {
            std::stringstream ss;
            ss << "Ziben::Singleton::GetRef: s_Instance(";
            ss << typeid(T).name();
            ss << ") is nullptr!";

            throw std::runtime_error(ss.str());
        }

        return *s_Instance;
    }

    template <typename T>
    T* Singleton<T>::GetPointer() {
        return s_Instance;
    }

} // namespace Ziben