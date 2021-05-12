set(TARGET ZibenEditor)

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.inl
)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_compile_definitions(ZIBEN_PROFILE)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE Source)

target_link_libraries(${TARGET} PRIVATE ZibenEngine)

add_custom_command(TARGET ${TARGET} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
    ${CMAKE_CURRENT_BINARY_DIR}/Assets
    COMMENT "Copy ZibenEditor/Assets files to build directory" VERBATIM
)

add_compile_options()