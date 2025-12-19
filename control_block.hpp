#pragma once
#include <iostream>
#include <string>
#include <utility>

template<typename T>
struct CB { 
    T* _data;
    size_t _s_count;
    size_t _w_count;
};