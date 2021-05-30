set(TARGET ZibenEngine)

add_subdirectory(External/entt)       # ecs library
add_subdirectory(External/freetype)   # freetype library
add_subdirectory(External/glew-cmake) # glew library
add_subdirectory(External/glfw)       # glfw library
add_subdirectory(External/glm)        # glm library
add_subdirectory(External/spdlog)     # logging library
add_subdirectory(External/yaml-cpp)   # serialization library

# Detect Debug / Release modes
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(ZIBEN_DEBUG)
    message(STATUS "ZibenEngine: Debug Mode")
else ()
    add_compile_definitions(ZIBEN_RELEASE)
    message(STATUS "ZibenEngine: Release Mode")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

MESSAGE(STATUS "ZIBEN PROF ${ZIBEN_ENABLE_PROFILING}")

# Enable Ziben Profiling
if (ZIBEN_ENABLE_PROFILING)
    add_compile_definitions(ZIBEN_PROFILING)
    message(STATUS "ZibenEngine: Enable Profiling")
endif (ZIBEN_ENABLE_PROFILING)

add_subdirectory(Source/Utility)
add_subdirectory(Source/Profiling)
add_subdirectory(Source/System)
add_subdirectory(Source/Window)
add_subdirectory(Source/Renderer)
add_subdirectory(Source/Scene)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/Ziben/*.hpp
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Source/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Source/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})
add_library(Ziben::Engine ALIAS ${TARGET})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Ziben/Include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/Source
)

target_link_libraries(${TARGET} PUBLIC ZibenWindow)

target_precompile_headers(${TARGET} PUBLIC Source/ZibenPch.hpp)