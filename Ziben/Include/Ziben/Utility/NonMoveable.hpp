#pragma once

#define NON_MOVEABLE(ClassName)                                                      \
    ClassName(ClassName&& other) = delete;                                           \
    ClassName& operator =(ClassName&& other) = delete;