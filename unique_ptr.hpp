#pragma once

#include <iostream>

template <typename T>
class unique_ptr
{
private:
    T* _data;

public:
    explicit unique_ptr(T* data = nullptr) : _data{data} {}
    unique_ptr(unique_ptr&& other) noexcept : _data{other._data}, { other._data = nullptr; }
    unique_ptr& operator =(unique_ptr&& other) noexcept { if (this != &other) { delete _data; _data = other._data; other._data = nullptr; }}
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator =(const unique_ptr&) = delete;
    ~unique_ptr() { delete _data; }

    // Modifiers
    T* release();
    void reset(T* data);
    void swap(const unique_ptr* other) { std::swap(_data, other)};

    // Observers
    T* get() { return _data; }
    explicit operator bool() const noexcept { return _data; }

    // Single-object version
    T& operator *() const noexcept { return *_data; }
    T* operator ->() const noexcept { return _data; }

    // Non-member functions
    friend unique_ptr make_unique(T data) { return unique_ptr<T>(new T(data)); }

    friend bool operator== (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs);
    friend bool operator!= (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs);
    friend bool operator< (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs);
    friend bool operator<= (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs);
    friend bool operator> (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs);
    friend bool operator>= (const unique_ptr<T>& lhs, const unique_ptr<T>& rhs);
};