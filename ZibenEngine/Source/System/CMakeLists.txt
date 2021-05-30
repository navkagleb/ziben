set(TARGET ZibenSystem)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/Ziben/System/*.hpp
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Source/System/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Source/System/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/Ziben/System
        ${PROJECT_SOURCE_DIR}/ZibenEngine/Include
)

target_link_libraries(${TARGET} PUBLIC ZibenUtility glfw spdlog)