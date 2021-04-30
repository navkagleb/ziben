set(TARGET ZibenProfiling)

file(GLOB HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Profiling/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Profiling/*.hpp
)

file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Profiling/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC ZibenUtility)

target_include_directories(${TARGET}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Profiling
        ${PROJECT_SOURCE_DIR}/Ziben/Include
)

target_precompile_headers(${TARGET} PUBLIC ProfilingPch.hpp)