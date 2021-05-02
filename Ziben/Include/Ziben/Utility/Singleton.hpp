#pragma once

#include "NonCopyable.hpp"
#include "NonMoveable.hpp"

namespace Ziben {

    template <typename T>
    class Singleton final {
    public:
        template <typename... Args>
        static T& Create(Args&&... args);

        static void Destroy();
        static inline T& GetRef();
        static inline T* GetPointer();

        NON_COPYABLE(Singleton);
        NON_MOVEABLE(Singleton);

    private:
        Singleton() = default;
        ~Singleton() = default;

        static inline std::unique_ptr<T> s_Instance;

    }; // class Singleton

} // namespace Ziben

#include "Singleton.inl"