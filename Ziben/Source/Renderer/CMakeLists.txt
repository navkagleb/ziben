set(TARGET ZibenRenderer)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Renderer/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Renderer/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Renderer/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/External/glew-cmake/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Renderer
        ${PROJECT_SOURCE_DIR}/Ziben/Include/
)

target_link_libraries(${TARGET} PUBLIC glfw libglew_static glm)

target_precompile_headers(${TARGET} PUBLIC RendererPch.hpp)