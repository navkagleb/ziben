namespace Ziben {

    template <typename T>
    template <typename... Args>
    T& Singleton<T>::Create(Args&&... args) {
        if (s_Instance) {
            std::stringstream ss;
            ss << "Ziben::Singleton<" << typeid(T).name() << ">::Create: s_Instance is not nullptr!";
            throw std::runtime_error(ss.str());
        }

        return *(s_Instance = std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <typename T>
    void Singleton<T>::Destroy() {
        s_Instance.reset();
    }

    template <typename T>
    inline T& Singleton<T>::GetRef() {
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
        return s_Instance.get();
    }

} // namespace Ziben