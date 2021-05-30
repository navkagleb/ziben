#pragma once

namespace Ziben {

    class FileDialogs {
    public:
        static void Init(void* nativeWindow);

        // Return empty strings if cancelled
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);

    private:
        struct Data {
            void* NativeWindow;
        };

    private:
        static Data& GetData();

    }; // class FileDialogs

} // namespace Ziben