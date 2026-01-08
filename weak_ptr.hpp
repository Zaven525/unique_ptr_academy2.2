#include "shared_ptr.hpp"
#include "control_block.hpp"
template <typename T>
class weak_ptr
{
public:
    constexpr weak_ptr() noexcept : _weakCB{nullptr} {}
    weak_ptr(const weak_ptr& other) noexcept;
    weak_ptr(const std::shared_ptr<T>& other) noexcept;
    weak_ptr(weak_ptr&& other) noexcept;
    weak_ptr& operator=(const weak_ptr& other) noexcept;
    weak_ptr& operator=(const shared_ptr<T>& other) noexcept;
    weak_ptr& operator=(weak_ptr&& other) noexcept;
    ~weak_ptr() { if(_weakCB) _weakCB->relaese_weak(); }

public:
    CB* get() const noexcept{ return _weakCB; }
    void reset() noexcept;
    void swap(weak_ptr<T>& other) { std::swap(*this, other); }
    size_t use_count() const noexcept;
private:
    CB* _weakCB;
};


template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr& other) noexcept : _weakCB{other._weakCB}
{
    if (_weakCB)
    {
        _weakCB->add_weak();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr( const std::shared_ptr<T>& other) noexcept
{
    _weakCB = other.getCB();
    if (_weakCB)
    {
        _weakCB->add_weak();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr&& other) noexcept : _weakCB{other.CB();}
{
    other._weakCB = nullptr;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr& other) noexcept
{
    if (this != &other)
    {
        if (_weakCB) _weakCB->release_weak();
        _weakCB = other._weakCB;
        if (_weakCB) _weakCB->add_weak();
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<T>& other) noexcept
{
    if (_weakCB) _weakCB->release_weak();
    _weakCB = other.getCB();
    if (_weakCB) _weakCB->add_weak();
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr&& other) noexcept
{
    if (this != &other)
    {
        if (_weakCB) _weakCB->release_weak();
        weakCB = other._weakCB;
        other._weakCB = nullptr;
    }
    return *this;
}

template <typename T>
void weak_ptr<T>::reset() noexcept
{
    if (_weakCB) _weakCB->release_weak();
    _weakCB = nullptr;
}

template <typename T>
size_t weak_ptr<T>::use_count() const noexcept
{
    if (_weakCB) return _weakCB->_get_use_count();
    else return 0;
}


