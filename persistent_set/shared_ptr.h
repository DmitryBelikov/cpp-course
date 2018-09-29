#ifndef SHARED_PTR_H
#define SHARED_PTR_H

template <typename T>
struct shared_ptr
{
public:
    shared_ptr(T* ptr);
    shared_ptr(shared_ptr& other);
    shared_ptr(shared_ptr&& other);
    shared_ptr& operator=(shared_ptr& other);

    ~shared_ptr();

private:
    struct pointer;
    pointer* ptr;
};

template <typename T>
struct shared_ptr<T>::pointer
{
public:
    pointer(T* object);

    size_t count;
    T* object;
};

template<typename T>
shared_ptr<T>::pointer::pointer(T* object)
    : count(1)
    , object(object)
{}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr& other)
{
    other->ptr.count++;
    ptr = other->ptr;
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other)
    : ptr(other.ptr)
{}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr& other)
{
    other->ptr.count++;
    ptr = other.ptr;
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
    if(ptr->count == 1)
        delete ptr;
    else
        --ptr->count;
}

#endif // SHARED_PTR_H

