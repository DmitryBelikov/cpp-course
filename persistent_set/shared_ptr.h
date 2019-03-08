#ifndef SHARED_PTR_H
#define SHARED_PTR_H

template <typename T>
struct shared_ptr
{
public:

    template<typename... Args>
    shared_ptr(const Args&... args);

    shared_ptr(const shared_ptr& other);
    shared_ptr(shared_ptr&& other);
    shared_ptr& operator=(const shared_ptr& other);

    ~shared_ptr();

private:
    void decrease_count();

    struct pointer;
    pointer* ptr;
};

template <typename T>
struct shared_ptr<T>::pointer
{
public:
    pointer(T* object);
    ~pointer();

    mutable size_t count;
    T* object;
};

template<typename T>
shared_ptr<T>::pointer::pointer(T* object)
    : count(1)
    , object(object)
{}

template<typename T>
shared_ptr<T>::pointer::~pointer()
{
    delete object;
}

template<typename T, typename... Args>
shared_ptr<T>::shared_ptr(const Args&... args)
    : ptr(new T(args))
{}


template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other)
{
    other.ptr->count++;
    ptr = other.ptr;
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other)
    : ptr(other.ptr)
{
    other.ptr = nullptr;
}

template<typename T>
void shared_ptr<T>::decrease_count()
{
    ptr->count--;
    if (ptr->count == 0)
        delete ptr;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other)
{
    if (ptr == other.ptr)
        return *this;

    other.ptr->count++;
    decrease_count();
    ptr = other.ptr;

    return *this;
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
    if (!ptr)
        return;
    decrease_count();
}

#endif // SHARED_PTR_H

