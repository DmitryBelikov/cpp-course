#ifndef LINKED_PTR_H
#define LINKED_PTR_H

template <typename T>
struct linked_ptr
{
public:
    linked_ptr(T &target);
    linked_ptr(const linked_ptr &other);
    linked_ptr& operator=(const linked_ptr &other);

    ~linked_ptr();
private:
    T* ptr;
    linked_ptr* left;
    linked_ptr* right;
};

template <typename T>
linked_ptr<T>::linked_ptr(T &target):
    ptr(target),
    left(nullptr),
    right(nullptr)
{}

template <typename T>
linked_ptr<T>::linked_ptr(const linked_ptr &other):
    ptr(other->ptr),
    right(other->right),
    left(other)
{
    if(other->right != nullptr)
        other->right->left = *this;
    other->right = *this;
}

template <typename T>
linked_ptr<T>::operator=(const linked_ptr &other)
{
    ptr = other->ptr;
    right = other->right;
    left = other;
    if(other->right != nullptr)
        other->right->left = *this;
    other->right = *this;
}

template <typename T>
linked_ptr<T>::~linked_ptr()
{
    if(left == nullptr && right == nullptr)
    {
        delete ptr;
        return;
    }

    if(left != nullptr)
        left->right = right;
    if(right != nullptr)
        right->left = left;
}

#endif // LINKED_PTR_H

