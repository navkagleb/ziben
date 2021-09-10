# Ziben Game Engine

## Description

Ziben - 2D Game Engine based on OpenGL

![ZibenEditor Example](Resources/ZibenEditor.gif)

## Installation

Application supports only Windows Operating System and supports only MinGW compiler

Download the latest version of [MinGW](https://winlibs.com/) Compiler.
Add compiler to PATH environment variable

Download the [CMake](https://cmake.org/download/). Add CMake to PATH environment variable

```bash
> git clone --recursive https://github.com/navkagleb/Labs/tree/master/Term4/OOP/Lab03
```

Go to created directory

```bash
> mkdir build
> cd build
> cmake .. -G "MinGW Makefiles"
> mingw32-make
```

Go to 'Source' directory and run the program

```bash
> cd Source
> ./SortAlgorithms
```

## Integration

* Ad submodule
```cmake
add_subdirectory(Ziben)                        # path to the 'random' library root
                                               # create target
target_link_libraries(${TARGET} ZibenEngine)   # add include path to a compiler
```

## Usage

```c++
#include <Ziben/Application.hpp>
#include <Ziben/Scene/Layer.hpp>

class Layer : public Ziben::Layer {
    void OnAttach() override {
        // Called to initialize the layer
    }
    
    void OnDetach() override {
        // Called before the layer is destroyed
    }
    
    void OnEvent(Ziben::Event& event) override {
        // Called to track window events
    }
    
    void OnUpdate(const Ziben::TimeStep& ts) override {
        // Call to update objects
    }
    
    void OnRender() override {
        // Called to render objects
    }
    
    void OnImGuiRender() override {
        // Called to render ImGui widgets
    }
};

class Application : public Ziben::Application {
public:
    Application() : Ziben::Application("Window", 1280, 720) {
        PushLayer(new Layer);
    }
};

Ziben::Application* Ziben::CreateApplication(int argc, char** argv) {
    (void)argc;
    (void)argv;

    return new ::Application();
}
```

## License
[MIT](https://choosealicense.com/licenses/mit/)
