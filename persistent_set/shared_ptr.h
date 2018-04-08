#ifndef SHARED_PTR_H
#define SHARED_PTR_H

template <typename T>
struct shared_ptr
{
public:
    shared_ptr(T &object);
    shared_ptr(const shared_ptr<T> &other);
    shared_ptr& operator=(const shared_ptr<T> &other);

    ~shared_ptr();
private:
    struct pointer;
    pointer* ptr;
};

template <typename T>
struct shared_ptr<T>::pointer
{
public:
    pointer();
    size_t count;
    T* object;
};

template<typename T>
shared_ptr<T>::pointer pointer(T &target):
    count(1),
    object(target)
{}

template <typename T>
shared_ptr<T>::shared_ptr(T &object)
{
    ptr = new pointer(object);
}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr &other)
{
    other->ptr.count++;
    ptr = other->ptr;
}

template <typename T>
shared_ptr<T>::operator=(const shared_ptr &other)
{
    other->ptr.count++;
    ptr = other.ptr;
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
    if(ptr.count == 1)
        delete ptr;
    else
        --count;
}

#endif // SHARED_PTR_H

