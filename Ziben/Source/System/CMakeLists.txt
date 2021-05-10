set(TARGET ZibenSystem)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/System/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Source/System/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/System/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/System
        ${PROJECT_SOURCE_DIR}/Ziben/Include
)

target_link_libraries(${TARGET} PUBLIC ZibenUtility glfw spdlog)