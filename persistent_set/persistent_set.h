#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <vector>
#include <iostream>
#include <memory>


using std::shared_ptr;
using std::pair;

//T should be comparable
template <typename T>
struct persistent_set {
public:
    struct iterator;

    persistent_set();
    persistent_set(const persistent_set& other);
    persistent_set& operator=(const persistent_set& other) noexcept;
    ~persistent_set();

    iterator find(const T& value);
    std::pair<typename persistent_set<T>::iterator, bool> insert(const T& value);
    void erase(iterator it);

    iterator begin() const;
    iterator end() const;

    bool empty() const;

private:
    struct node_t;

    node_t* copy_path(node_t* last);

    std::unique_ptr< node_t > root;
};

template<typename T>
struct persistent_set<T>::iterator {
public:
    T const& operator*() const;

    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);

private:
    iterator(node_t* node);
    node_t* node;

    friend struct persistent_set;
};

template<typename T>
struct persistent_set<T>::node_t {
public:
    node_t() = delete;
    node_t(const T& value);
    node_t(const node_t& other);
    node_t& operator=(const node_t& other);


    T value;

    node_t* parent;
    shared_ptr< node_t > left;
    shared_ptr< node_t > right;
};

template<typename T>
persistent_set<T>::persistent_set() 
    : root(nullptr)
{}

template<typename T>
persistent_set<T>::persistent_set(const persistent_set<T>& other)
{
    if (other.root.get()) {
        root.reset(new node_t(other.root->value));
        root->left = other.root->left;
        root->right = other.root->right;
    }
    else {
        root.reset(nullptr);
    }
}

template<typename T>
persistent_set<T>& persistent_set<T>::operator=(const persistent_set<T>& other) noexcept {
    if (this == &other)
        return *this;
    if (other.root.get()) {
        root->value = other.root->value;
        root->left = other.root->left;
        root->right = other.root->right;
    } else {
        root.reset(nullptr);
    }
    return *this;
}

template<typename T>
persistent_set<T>::~persistent_set() {
}

template<typename T>
typename persistent_set<T>::iterator persistent_set<T>::find(const T& value) {
    if (empty())
        return end();
    node_t* current = root.get();
    while (current) {
        if (current->value == value)
            return iterator(current);
        if (current->value > value)
            current = current->left.get();
        else
            current = current->right.get();
    }
    return end();
}

template<typename T>
typename persistent_set<T>::node_t* persistent_set<T>::copy_path(typename persistent_set<T>::node_t* last)
{
    persistent_set<T> other = *this;
    node_t* old_current = root.get();
    node_t* new_current = other.root.get();
    T value = last->value;
    
    while (old_current != last) {
        node_t* current_copy;
        node_t* added;
        if (old_current->value > value) {
            current_copy = old_current->left.get();
            added = new node_t(*current_copy);
            new_current->left.reset(added);
        } else {
            current_copy = old_current->right.get();
            added = new node_t(*current_copy);
            new_current->right.reset(added);
        }
        added->parent = new_current;
        new_current = added;
        old_current = current_copy;
    }

    return new_current;
}

template<typename T>
std::pair<typename persistent_set<T>::iterator, bool> persistent_set<T>::insert(const T& value) {
    if (empty()) {
        persistent_set<T> other;
        other.root.reset(new node_t(value));
        return{ iterator(other.root.get()), true };
    }
    node_t* current = root.get();
    node_t* last = nullptr;
    bool is_less;
    while (current) {
        if (current->value == value) {
            return{ iterator(current), false };
        }
        is_less = current->value > value;
        if (is_less)
            current = current->left.get();
        else
            current = current->right.get();
    }
    node_t* copy = copy_path(last);
    node_t* added;
    if (is_less) {
        copy->left.reset(new node_t(value));
        added = copy->left.get();
    } else {
        copy->right.reset(new node_t(value));
        added = copy->right.get();
    }

    added->parent = copy;
    return{ iterator(added), true };
}

template<typename T>
void persistent_set<T>::erase(iterator it) {
    node_t* node = it->node;
    T value = node->value;
    node* current = root;
    while (current) {
        if (current->value == value) {
            //node* copy_path()
        }
        break;
    }
}

template<typename T>
typename persistent_set<T>::iterator persistent_set<T>::begin() const {
    return iterator(root.get());
}

template<typename T>
typename persistent_set<T>::iterator persistent_set<T>::end() const {
    return iterator(nullptr);
}

template<typename T>
bool persistent_set<T>::empty() const {
    return root.get() ? true : false;
}

template<typename T>
persistent_set<T>::node_t::node_t(const T& value) 
    : value(value)
    , parent(nullptr)
    , left(nullptr)
    , right(nullptr)
{}

#endif //PERSISTENT_SET_H
