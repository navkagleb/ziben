set(TARGET ZibenRenderer)

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

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
        ${PROJECT_SOURCE_DIR}/Ziben/External/glew-cmake/include
    PRIVATE
        # stb library
        ${PROJECT_SOURCE_DIR}/Ziben/External/stb

        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Renderer
        ${PROJECT_SOURCE_DIR}/Ziben/Include/
)

target_link_libraries(${TARGET} PUBLIC ZibenSystem ZibenProfiling libglew_static glm EnTT::EnTT)
#target_link_libraries(${TARGET} PUBLIC OpenGL::OpenGL OpenGL::EGL)

#-lglut32 -lglu32 -lopengl32

target_precompile_headers(${TARGET} PUBLIC RendererPch.hpp)