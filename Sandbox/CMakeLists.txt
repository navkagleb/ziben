set(TARGET Sandbox)

file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.hpp)
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ZibenEngine)