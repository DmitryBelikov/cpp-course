#ifndef LINKED_PTR_H
#define LINKED_PTR_H

template <typename T>
struct linked_ptr
{
public:
    linked_ptr() noexcept;
    linked_ptr(T* target) noexcept;
    linked_ptr(const linked_ptr& other) noexcept;
    linked_ptr(linked_ptr&& other) noexcept;
    linked_ptr& operator=(const linked_ptr& other) noexcept;
    linked_ptr& operator=(linked_ptr&& other) noexcept;

    ~linked_ptr();

    T& operator*();
    T* operator->();

    T* get() const noexcept;
    void reset(T* object) noexcept;

private:
    void call_deleter();

    T* ptr;
    T* stored_ptr = nullptr;

    mutable linked_ptr* left;
    mutable linked_ptr* right;
};

template<typename T>
linked_ptr<T>::linked_ptr() noexcept
    : ptr(nullptr)
    , left(nullptr)
    , right(nullptr)
{}

template <typename T>
linked_ptr<T>::linked_ptr(T* target) noexcept
    : ptr(target)
    , left(nullptr)
    , right(nullptr)
{}

template <typename T>
linked_ptr<T>::linked_ptr(const linked_ptr<T>& other) noexcept
    : ptr(other.ptr)
    , left(const_cast< linked_ptr<T>* >(&other))
    , right(other.right)
{
    if (right)
        right->left = this;
    other.right = this;
}

template <typename T>
linked_ptr<T>::linked_ptr(linked_ptr&& other) noexcept
    : ptr(other.ptr)
    , left(other.left)
    , right(other.right)
{
    if (left)
        left->right = this;

    if (right)
        right->left = this;

    other.ptr = nullptr;
    other.left = nullptr;
    other.right = nullptr;
}

template <typename T>
linked_ptr<T>& linked_ptr<T>::operator=(const linked_ptr& other) noexcept
{
    if (ptr == other.ptr)
        return *this;

    if (right)
        right->left = left;
    if (left)
        left->right = right;

    if (!left && !right)
        stored_ptr = ptr;

    ptr = other.ptr;
    right = other.right;
    left = const_cast<linked_ptr<T>*>(&other);

    if (right)
        right->left = this;
    other.right = this;

    call_deleter();

    return *this;
}

template<typename T>
linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr&& other) noexcept {
    if(ptr == other.ptr) {
        return this;
    }

    if (right)
        right->left = left;
    if (left)
        left->right = right;

    if (!left && !right)
        stored_ptr = ptr;

    ptr = other.ptr;
    right = other.right;
    left = other.left;

    if (right)
        right->left = this;
    if (left)
        left->right = this;
    other.left = nullptr;
    other.right = nullptr;
    other.ptr = nullptr;

    call_deleter();

    return *this;
}


template<typename T>
void linked_ptr<T>::call_deleter() {
    if(stored_ptr != nullptr) {
        delete stored_ptr;
        stored_ptr = nullptr;
    }
}

template <typename T>
linked_ptr<T>::~linked_ptr()
{
    if (!left && !right)
    {
        if(ptr)
            delete ptr;
        return;
    }

    if (left)
        left->right = right;
    if (right)
        right->left = left;
}

template <typename T>
T& linked_ptr<T>::operator*()
{
    return *ptr;
}

template <typename T>
T* linked_ptr<T>::operator->()
{
    return ptr;
}

template <typename T>
T* linked_ptr<T>::get() const noexcept {
    return ptr;
}

template <typename T>
void linked_ptr<T>::reset(T* object) noexcept {
    linked_ptr<T> temp(object);
    *this = temp;
}

#endif // LINKED_PTR_H

