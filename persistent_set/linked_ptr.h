#ifndef LINKED_PTR_H
#define LINKED_PTR_H

template <typename T>
struct linked_ptr
{
public:
    linked_ptr(T* target);
    linked_ptr(const linked_ptr& other);
    linked_ptr(linked_ptr&& other);
    linked_ptr& operator=(const linked_ptr& other);
    ~linked_ptr();

    T& operator*();
    T* operator->();

private:
    T* ptr;
    mutable linked_ptr* left;
    mutable linked_ptr* right;
};

template <typename T>
linked_ptr<T>::linked_ptr(T* target)
    : ptr(target)
    , left(nullptr)
    , right(nullptr)
{}

template <typename T>
linked_ptr<T>::linked_ptr(const linked_ptr& other)
    : ptr(other.ptr)
    , left(const_cast< linked_ptr< int >* >(&other))
    , right(other.right)
{
    if (right)
        right->left = this;
    other.right = this;
}

template <typename T>
linked_ptr<T>::linked_ptr(linked_ptr&& other)
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
linked_ptr<T>& linked_ptr<T>::operator=(const linked_ptr& other)
{
    if (ptr == other.ptr)
        return *this;

    if (right)
        right->left = left;
    if (left)
        left->right = right;

    if (!left && !right)
        delete ptr;

    ptr = other.ptr;
    right = other.right;
    left = const_cast<linked_ptr< int > *>(&other);

    if (right)
        right->left = this;
    other.right = this;

    return *this;
}

template <typename T>
linked_ptr<T>::~linked_ptr()
{
    if (!left && !right)
    {
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

#endif // LINKED_PTR_H

