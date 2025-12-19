#pragma once
#include <iostream>
#include <string>
#include <utility>
#include "unique_ptr.hpp"
#include "control_block.hpp"
template <typename T>
class shared_ptr
{
private:
    T* _data;
    CB<T>* _sh_control_block;
public:
    // Special functions
    explicit shared_ptr(T* data = nullptr);
    shared_ptr(const shared_ptr<T>& other) noexcept;
    shared_ptr(shared_ptr<T>&& other) noexcept;
    shared_ptr(unique_ptr<T>& other) noexcept : shared_ptr(other._data.release()) {}
    shared_ptr& operator =(const shared_ptr<T>& other);
    shared_ptr& operator =(unique_ptr<T>& other) noexcept;    
    //shared_ptr& operator =(unique_ptr&& other) noexcept;
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
    size_t use_count() const noexcept { return _sh_control_block->strong_count; }
    bool unique() const noexcept { return _sh_control_block->strong_count == 1; }
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
template<typename T>
shared_ptr<T>::shared_ptr(T* data) : _data{data}
{
    if (data) 
    {
        _sh_control_block = new CB<T>;
        _sh_control_block->_data = data;
        _sh_control_block->_s_count = 1;
        _sh_control_block->_w_count = 0;
    }
    else 
    {
        _sh_control_block = nullptr;
    }

}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other) noexcept
{
    _data = other->_data;
    _sh_control_block = other._sh_control_block;
    _sh_control_block->_s_count++;
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other) noexcept
{
    
}




//     shared_ptr& operator =(const unique_ptr& other) noexcept;    
//     shared_ptr(shared_ptr<T>&& ohter) noexcept;
//     shared_ptr& operator =(unique_ptr&& other) noexcept;
//     shared_ptr& operator =(std::nullptr_t);
//     ~shared_ptr();

