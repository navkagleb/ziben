set(TARGET ZibenSandbox)

file(GLOB HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Algorithms/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Algorithms/*.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Application/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Layers/*.hpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/*.hpp
)

file(GLOB SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Algorithms/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Application/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entities/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Layers/*.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scene/*.cpp
)

#set(ZIBEN_PROFILE TRUE CACHE INTERNAL "")
add_compile_definitions(ZIBEN_PROFILE)

include_directories(Source)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ZibenEngine)

file(COPY
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    DESTINATION
        ${CMAKE_CURRENT_BINARY_DIR}
)