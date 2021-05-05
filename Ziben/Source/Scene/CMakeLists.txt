set(TARGET ZibenScene)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Scene/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Scene/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Scene/*.cpp

    # ImGui
    ${PROJECT_SOURCE_DIR}/Ziben/External/imgui/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Ziben/External/imgui
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/External/imgui/backends
        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Scene
        ${PROJECT_SOURCE_DIR}/Ziben/Include/
)

target_link_libraries(${TARGET}
    PUBLIC
        ZibenRenderer
    PRIVATE
        -loleaut32 -limm32 -lversion
)

target_precompile_headers(${TARGET} PUBLIC ScenePch.hpp)