set(TARGET ZibenEngine)

file(GLOB HEADER_FILES
    Include/Ziben/*.hpp
    Include/Ziben/Event/*.hpp
    Include/Ziben/Graphics/*.hpp
    Include/Ziben/System/*.hpp
    Include/Ziben/Utility/*.hpp
)

file(GLOB SOURCE_FILES
    Include/Ziben/*.cpp
    Include/Ziben/Event/*.cpp
    Include/Ziben/Graphics/*.cpp
    Include/Ziben/System/*.cpp
    Source/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/External/glew-cmake/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(${TARGET} PUBLIC spdlog glfw libglew_static glm)

target_precompile_headers(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/ZibenPch.hpp)