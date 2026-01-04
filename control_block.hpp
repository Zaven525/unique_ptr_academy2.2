#pragma once

template<typename T>
class CB 
{
    public:
        CB(T* data) : _data{data}, _use_count{1}, _weak_count{1} {}
        virtual ~CB() {}
        size_t get_count() { return _use_count; }
    public:
        void add_count() { _use_count++; }
        void add_weak() { _weak_count++; }
        void release_count();
        void release_weak();
        virtual void destroy_data() noexcept { delete _data; }
        virtual void destroy_self() noexcept { delete this; }
    private: 
        T* _data;
        size_t _use_count;
        size_t _weak_count;
};

template<typename T>
void CB<T>::release_count() 
{ 
    if(--_use_count == 0) 
    { 
        destroy_data(); 
        release_weak(); 
    } 
}

template<typename T>
void CB<T>::release_weak()
{
    if (--_weak_count == 0 )
    {
        destroy_self();
    }
}
