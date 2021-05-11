set(TARGET ZibenScene)

add_compile_definitions(IMGUI_ENABLE_FREETYPE)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Scene/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Scene/*.inl
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Scene/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Scene/*.cpp

    # ImGui
    ${PROJECT_SOURCE_DIR}/Ziben/External/imgui/*.cpp
    ${PROJECT_SOURCE_DIR}/Ziben/External/imgui/misc/freetype/*.cpp

    #ImGuizmo
    ${PROJECT_SOURCE_DIR}/Ziben/External/ImGuizmo/ImGuizmo.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Ziben/External/imgui
        ${PROJECT_SOURCE_DIR}/Ziben/External/ImGuizmo
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/External/imgui/backends
        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Scene
        ${PROJECT_SOURCE_DIR}/Ziben/Include/
)

target_link_libraries(${TARGET}
    PUBLIC
        ZibenRenderer EnTT::EnTT freetype yaml-cpp
    PRIVATE
        -loleaut32 -limm32 -lversion
)

target_precompile_headers(${TARGET} PUBLIC ScenePch.hpp)