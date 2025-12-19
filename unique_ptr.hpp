#pragma once
#include <iostream>
#include <string>
#include <utility>

template <typename T>
class unique_ptr
{
private:
    T* _data;

public:
    explicit unique_ptr(T* data = nullptr) : _data{data} {}
    unique_ptr(unique_ptr&& other) noexcept : _data{other._data} { other._data = nullptr; }
    unique_ptr& operator =(unique_ptr&& other) noexcept { if (this != &other) { delete _data; _data = other._data; other._data = nullptr; } return *this;}
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator =(const unique_ptr&) = delete;
    unique_ptr& operator =(std::nullptr_t) { reset(nullptr); return *this;}
    ~unique_ptr() { delete _data; }

    // Modifiers
    T* release();
    void reset() noexcept { delete _data; }
    void reset(T* data) ;
    void swap(unique_ptr& other) { std::swap(_data, other._data);}

    // Observers
    T* get() const { return _data; }
    explicit operator bool() const noexcept { return _data; }

    // Single-object version
    T& operator *() const noexcept { return *_data; }
    T* operator ->() const noexcept { return _data; }

    // Non-member functions

    friend bool operator== (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) { return lhs.get() == rhs.get(); }
    friend bool operator!= (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) { return lhs.get() != rhs.get(); }
    friend bool operator< (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) { return lhs.get() < rhs.get(); }
    friend bool operator<= (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) { return lhs.get() <= rhs.get(); }
    friend bool operator> (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) {return lhs.get() > rhs.get(); }
    friend bool operator>= (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) {return lhs.get() >= rhs.get(); }
    friend std::ostream& operator<<(std::ostream& os, const unique_ptr<T>& src) { return os << src.get(); }
};



template <typename T>
T* unique_ptr<T>::release()
{
    T* tmp = _data;
    _data = nullptr;
    return tmp;
}

template <typename T>
void unique_ptr<T>::reset(T* data)
{
    if (_data != data) {
        delete _data;
        _data = data;
    }
}



