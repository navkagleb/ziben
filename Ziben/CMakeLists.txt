set(TARGET ZibenEngine)

file(GLOB HEADER_FILES
    Include/Ziben/*.hpp
    Include/Ziben/Core/*.hpp
    Include/Ziben/Event/*.hpp
    Include/Ziben/Graphics/*.hpp
    Include/Ziben/ImGui/*.hpp
    Include/Ziben/Scene/*.hpp
    Include/Ziben/System/*.hpp
    Include/Ziben/Utility/*.hpp
)

file(GLOB SOURCE_FILES
    # ImGui
    ${PROJECT_SOURCE_DIR}/External/imgui/*.cpp

    Include/Ziben/*.cpp
    Include/Ziben/Core/*.cpp
    Include/Ziben/Event/*.cpp
    Include/Ziben/Graphics/*.cpp
    Include/Ziben/ImGui/*.cpp
    Include/Ziben/Scene/*.cpp
    Include/Ziben/System/*.cpp

    Source/*.cpp
    Source/ImGui/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/External/imgui
        ${PROJECT_SOURCE_DIR}/External/glew-cmake/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/External/imgui/backends
        ${PROJECT_SOURCE_DIR}/External/imgui/examples
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(${TARGET}
    PUBLIC
        spdlog glfw libglew_static glm
    PRIVATE
        -loleaut32 -limm32 -lversion
)

target_precompile_headers(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/ZibenPch.hpp)