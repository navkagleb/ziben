set(TARGET ZibenEditor)

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.inl
)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_compile_definitions(ZIBEN_PROFILE)

include_directories(Source)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ZibenEngine)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})