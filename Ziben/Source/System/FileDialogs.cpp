#include "Ziben/System/FileDialogs.hpp"

#ifdef ZIBEN_PLATFORM_WINDOWS
    #include <windows.h>

    #include <GLFW/glfw3.h>

    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#endif

namespace Ziben {

    void FileDialogs::Init(void* nativeWindow) {
        GetData().NativeWindow = nativeWindow;
    }

    std::string FileDialogs::OpenFile(const char* filter) {
        OPENFILENAMEA ofn;

        char szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        ofn.lStructSize  = sizeof(OPENFILENAME);
        ofn.hwndOwner    = glfwGetWin32Window((GLFWwindow*)GetData().NativeWindow);
        ofn.lpstrFile    = szFile;
        ofn.nMaxFile     = sizeof(szFile);
        ofn.lpstrFilter  = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        return GetOpenFileNameA(&ofn) == TRUE ? ofn.lpstrFile : std::string();
    }

    std::string FileDialogs::SaveFile(const char* filter) {
        OPENFILENAMEA ofn;

        char szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        ofn.lStructSize  = sizeof(OPENFILENAME);
        ofn.hwndOwner    = glfwGetWin32Window((GLFWwindow*)GetData().NativeWindow);
        ofn.lpstrFile    = szFile;
        ofn.nMaxFile     = sizeof(szFile);
        ofn.lpstrFilter  = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        return GetSaveFileNameA(&ofn) == TRUE ? ofn.lpstrFile : std::string();
    }

    FileDialogs::Data& FileDialogs::GetData() {
        static FileDialogs::Data data;
        return data;
    }

} // namespace Ziben