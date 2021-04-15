#pragma once

#define NON_COPYABLE(ClassName)                                                         \
    ClassName(const ClassName& other) = delete;                                         \
    ClassName& operator =(const ClassName& other) = delete;