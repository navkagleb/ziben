set(TARGET ZibenUtility)

file(GLOB_RECURSE HEADER_FILES
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/Ziben/Utility/*.hpp
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/Ziben/Utility/*.inl
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Source/Utility/*.hpp
)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/ZibenEngine/Source/Utility/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ZibenEngine/Include/Ziben/Utility
)

target_precompile_headers(${TARGET} PUBLIC UtilityPch.hpp)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)