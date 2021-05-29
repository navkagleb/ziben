set(TARGET Lab03)

file(GLOB HEADER_FILES ./*.hpp)
file(GLOB SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ZibenEngine)

file(COPY
        ${PROJECT_SOURCE_DIR}/External/Ziben/Ziben/Assets
    DESTINATION
        ${CMAKE_CURRENT_BINARY_DIR}
)