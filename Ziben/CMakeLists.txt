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
else ()
    add_compile_definitions(ZIBEN_RELEASE)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

add_subdirectory(Source/Utility)
add_subdirectory(Source/Profiling)
add_subdirectory(Source/System)
add_subdirectory(Source/Window)
add_subdirectory(Source/Renderer)
add_subdirectory(Source/Scene)

if (ZIBEN_PROFILE)
    add_compile_definitions(ZIBEN_PROFILE)
endif (ZIBEN_PROFILE)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Source/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Ziben/Include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/Source
)

target_link_libraries(${TARGET} PUBLIC ZibenWindow)

target_precompile_headers(${TARGET} PUBLIC Source/ZibenPch.hpp)