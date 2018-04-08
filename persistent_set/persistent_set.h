#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <vector>
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::pair;

//T should be comparable
template <typename T>
struct persistent_set
{
public:
    struct iterator;

    persistent_set();
    persistent_set(persistent_set const&);
    persistent_set& operator=(persistent_set const&);
    ~persistent_set();

    iterator find(T);
    pair<iterator, bool> insert(T const&);
    void erase(iterator);

    iterator begin() const;
    iterator end() const;

private:
    struct node;
    struct data;

    shared_ptr<node> root;
};

template <typename T>
struct persistent_set<T>::iterator
{
public:
    T const& operator*() const;

    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);

    friend bool operator==(persistent_set<T>::iterator const& a, persistent_set<T>::iterator const& b) {
        return a.vertex == b.vertex;
    }

    friend bool operator!=(persistent_set<T>::iterator const& a, persistent_set<T>::iterator const& b) {
        return a.vertex != b.vertex;
    }

private:
    node* vertex;
};

template <typename T>
struct persistent_set<T>::node
{
public:
    node();
    node(T const&);
    virtual ~node();

    node* parent;
    shared_ptr<node> l;
    shared_ptr<node> r;
};

template <typename T>
struct persistent_set<T>::data : node
{
public:
    data(T const&);
    T value;
};

template <typename T>
persistent_set<T>::persistent_set()
{
    root = shared_ptr<node>(nullptr);
}

template <typename T>
persistent_set<T>::persistent_set(persistent_set const& other)
{
    root = other.root;
}

template <typename T>
persistent_set<T>& persistent_set<T>::operator=(persistent_set const& other)
{
    root = other.root;
    return *this;
}

template <typename T>
persistent_set<T>::iterator persistent_set<T>::find(T const& value)
{
    if(root.get() == nullptr)
        return end();
    data* cur = static_cast<data*>(root.get());
    for(;;)
    {
        if(cur == nullptr)
            return end();
        if(cur->value == value)
            return iterator(static_cast<node*>(cur));
        if(cur->value < value)
            cur = static_cast<data*>(cur->r.get());
        else
            cur = static_cast<data*>(cur->l.get());
    }
}

template <typename T>
pair <persistent_set<T>::iterator, bool> persistent_set<T>::insert(T const& value)
{
    if(root.get() == nullptr) {
        persistent_set a;
        a.root.reset(new data(value));
        return {iterator(a.root.get()), true};
    }
    auto it = find(value);
    if(it != end())
        return {it, false};
    data* cur = static_cast<data*>(root.get());
    for(;;)
    {

    }
}

template <typename T>
void persistent_set<T>::erase(iterator it) {

}

#endif //PERSISTENT_SET_H
