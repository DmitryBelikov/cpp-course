#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <cassert>

template <typename T>
struct shared_ptr {
public:
    shared_ptr();
    shared_ptr(T* ptr);
    shared_ptr(const shared_ptr& other);
    shared_ptr(shared_ptr&& other);
    shared_ptr& operator=(const shared_ptr& other);

    ~shared_ptr();

    T* get() const;
    void reset(T* ptr);

private:
    void decrease_count();
    void call_deleter();

    struct pointer;
    pointer* ptr;

    pointer* stored_ptr = nullptr;
};

template <typename T>
struct shared_ptr<T>::pointer {
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
shared_ptr<T>::pointer::~pointer() {
    if(object)
        delete object;
}

template <typename T>
shared_ptr<T>::shared_ptr()
    : ptr(new pointer(nullptr))
{}

template <typename T>
shared_ptr<T>::shared_ptr(T* object)
    : ptr(new pointer(object))
{}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other)
    : ptr(other.ptr) {
    if(ptr) {
        ptr->count++;
    }
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other)
    : ptr(other.ptr) {
    other.ptr = nullptr;
}

template<typename T>
void shared_ptr<T>::decrease_count() {
    if(!ptr)
        return;
    assert(ptr->count > 0);
    ptr->count--;
    if(ptr->count == 0) {
        stored_ptr = ptr;
    }
}

template<typename T>
void shared_ptr<T>::call_deleter() {
    if(stored_ptr != nullptr) {
        delete stored_ptr;
        stored_ptr = nullptr;
    }
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other) {
    if (ptr == other.ptr)
        return *this;

    decrease_count();
    ptr = other.ptr;
    if(ptr)
        ptr->count++;
    call_deleter();
    return *this;
}

template <typename T>
shared_ptr<T>::~shared_ptr() {
    decrease_count();
    call_deleter();
}

template <typename T>
T* shared_ptr<T>::get() const {
    if(!ptr)
        return nullptr;
    return ptr->object;
}

template <typename T>
void shared_ptr<T>::reset(T* object) {
    shared_ptr<T> temp(object);
    *this = temp;
}

#endif // SHARED_PTR_H

