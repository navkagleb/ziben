set(TARGET ZibenUtility)

file(GLOB_RECURSE HEADER_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Utility/*.hpp
    ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Utility/*.inl
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Utility/*.hpp
)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/Ziben/Source/Utility/*.cpp
)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Ziben/Include/Ziben/Utility
        ${PROJECT_SOURCE_DIR}/Ziben/Include/
)

target_precompile_headers(${TARGET} PUBLIC UtilityPch.hpp)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)