#pragma once
#include <iostream>
#include <string>
#include <utility>
#include "unique_ptr.hpp"
#include "control_block.hpp"
template <typename T>
class shared_ptr
{
public:
    // Special functions
    explicit shared_ptr(T* data = nullptr);
    shared_ptr(const shared_ptr<T>& other) noexcept;
    shared_ptr(shared_ptr<T>&& other) noexcept;
    shared_ptr(unique_ptr<T>&& other) noexcept : shared_ptr(other._data.release()) {}
    shared_ptr& operator =(const shared_ptr<T>& other);
    shared_ptr& operator =(unique_ptr<T>&& other) noexcept;
    shared_ptr& operator =(std::nullptr_t);
    ~shared_ptr() noexcept;

public:
    // Modifiers
    void reset(T* data = nullptr);
    void swap( shared_ptr& r ) noexcept { std::swap(*this, r); }

    // Observers
    T* get() { return _data; }
    CB* getCB() { return _sharedCB; }

    T& operator *() const noexcept { return *_data; }
    T* operator ->() const noexcept { return _data; }
    size_t use_count() const noexcept { return _sharedCB->get_count(); }
    bool unique() const noexcept { return _sharedCB->_use_count == 1; }
    explicit operator bool() const noexcept { return _data != nullptr; }
    
    // Non-member functions
    friend bool operator== (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() == rhs.get(); }
    friend bool operator!= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() != rhs.get(); }
    friend bool operator< (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() < rhs.get(); }
    friend bool operator<= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) { return lhs.get() <= rhs.get(); }
    friend bool operator> (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) {return lhs.get() > rhs.get(); }
    friend bool operator>= (const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) {return lhs.get() >= rhs.get(); }
    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<T>& src) { return os << src.get(); }

private:
    T* _data;
    CB<T>* _sharedCB;
};

// Special functions
template<typename T>
shared_ptr<T>::shared_ptr(T* data) : _data{data}
{
    if (_data) 
    {
        _sharedCB = new CB<T>(_data);
    }
    else 
    {
        _sharedCB = nullptr;
    }

}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other) noexcept 
    : _data{other._data}, 
      _sharedCB{other._sharedCB}
{
    if (_sharedCB) _sharedCB->add_count();
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other) noexcept
    : _data{other._data}, 
      _sharedCB{other._sharedCB}
{
    other._data = nullptr;
    other._sharedCB = nullptr;
}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& other)
{
    if (this != &other)
    {
        if(_sharedCB) _sharedCB->release_count();
        _data = other._data;
        _sharedCB = other._sharedCB;
        if(_sharedCB) _sharedCB->add_count();
    }
    return *this;
}



template<typename T>
shared_ptr<T>& shared_ptr<T>::operator =(unique_ptr<T>&& other) noexcept
{
    if (_sharedCB) _sharedCB->release_count();
    _data = other.release();
    if(_data) 
    {
        _sharedCB = new CB<T>(_data);
    } 
    else
    {
        _sharedCB = nullptr;
    }
    return *this;
}

template<typename T>
shared_ptr<T>::~shared_ptr() noexcept
{
   if(_sharedCB) _sharedCB->release_count();
   _data = nullptr;
   _sharedCB = nullptr;
}



//Modifiers
template<typename T>
void shared_ptr<T>::reset(T* data)
{
    if (_sharedCB) _sharedCB->release_count();

    _data = data;
    if (_data)
    {
        _sharedCB = new CB<T>(_data);
    }
    else
    {
        _sharedCB = nullptr;
    }
}


