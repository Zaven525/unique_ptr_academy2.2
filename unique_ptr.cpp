#include "unique_ptr.hpp"

// Modifiers
template <typename T>
T* unique_ptr<T>::release()
{
    T* tmp = _data;
    _data = nullptr;
    return tmp;
}

template <typename T>
void unique_ptr<T>::reset(T* data = nullptr)
{
    if (_data != data) {
        delete _data;
        _data = data;
    }
}


// Non-member functions
