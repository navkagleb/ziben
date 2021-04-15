cmake_minimum_required(VERSION 3.19)
project(Ziben)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(External/glfw)       # glfw library
add_subdirectory(External/glew-cmake) # glew library
add_subdirectory(External/glm)        # glm library

add_subdirectory(Ziben)               # Ziben GameEngine
add_subdirectory(Sandbox)             # Subdirectory to test Ziben GameEngine