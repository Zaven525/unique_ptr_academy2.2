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
    void reset(T* data);
    void swap(unique_ptr* other) { std::swap(*this, other);}

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


template <typename T>
class shared_ptr<T>
{
private:
    struct CB { size_t strong_count; };
    CB* _control_block;
    T* _data;
public:
    // Special functions
    explicit shared_ptr(T* data = nullptr);
    shared_ptr(const shared_ptr<T>& other) noexcept;
    shared_ptr& operator =(const unique_ptr& other) noexcept;    
    shared_ptr(shared_ptr<T>&& ohter) noexcept;
    shared_ptr& operator =(unique_ptr&& other) noexcept;
    shared_ptr& operator =(std::nullptr_t);
    ~shared_ptr();

    // Modifiers
    void reset() noexcept { delete _data; }
    void reset(T* data);
    void swap( shared_ptr& r ) noexcept { std::swap(*this, r); }

    // Observers
    void get() { return _data; }
    T& operator *() const noexcept { return *_data; }
    T* operator ->() const noexcept { return _data; }
    size_t use_count() const noexcept { return _control_block->strong_count; }
    bool unique() const noexcept { return _control_block->strong_count == 1; }
    explicit operator bool() const noexcept { return get() != nullptr; }
    
    
    // Non-member functions
    friend bool operator== (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() == rhs.get(); }
    friend bool operator!= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() != rhs.get(); }
    friend bool operator< (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() < rhs.get(); }
    friend bool operator<= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() <= rhs.get(); }
    friend bool operator> (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) {return lhs.get() > rhs.get(); }
    friend bool operator>= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) {return lhs.get() >= rhs.get(); }
    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<T>& src) { return os << src.get(); }
};

// Special functions
shared_ptr::shared_ptr(T* data = nullptr)
{
    if (data == nullptr)
    {
        _data = nullptr;
        _control_block = nullptr;
    }
    else
    {
        _data = data;
        _control_block = new CB{1};
    }
}

shared_ptr& shared_ptr::operator=(const shared_ptr<T>& other)
{
    if (this != &other)
    {
        _data = other._data;
        _control_block = other._control_block;
        _control_block->strong_count++;
    }
    return *this;
}

shared_ptr& shared_ptr::operator=(shared_ptr<T>&& other)
{
    if (this != &other)
    {
        delete _data;
        --_control_block;
        _data = other._data;
        _control_block = other._control_block;
        other._data = nullptr;
    }
}