#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <vector>
#include <iostream>
#include <memory>
#include "shared_ptr.h"

template <typename T, template <class> class P = shared_ptr>
struct persistent_set {
public:
    struct iterator;

    persistent_set() = default;
    persistent_set(const persistent_set& other) = default;
    persistent_set& operator=(const persistent_set& other) = default;
    ~persistent_set() = default;

    iterator find(const T& value);
    std::pair<iterator, bool> insert(const T& value);
    void erase(iterator it);

    iterator begin() const;
    iterator end() const;

    bool empty() const;

private:
    struct node_t;

    iterator copy(iterator it);

    P<node_t> root;
};

template <typename T, template <class> class P>
struct persistent_set<T, P>::iterator {
public:
    iterator() = default;

    T const& operator*() const;

    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other);

private:
    iterator(const std::vector<node_t*>& nodes, persistent_set<T, P>* owner);
    iterator(const std::vector<P<node_t>>& nodes, persistent_set<T, P>* owner);

    std::vector<node_t*> path;
    persistent_set<T, P>* owner;

    friend struct persistent_set;
};

template <typename T, template <class> class P>
struct persistent_set<T, P>::node_t {
public:
    node_t() = delete;
    node_t(const T& value);
    node_t(const node_t& other) = default;
    node_t& operator=(const node_t& other) = default;

    T value;
    std::array<P<node_t>, 2> children;
};

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::find(const T& value) {
    std::vector<node_t*> path;
    node_t* cur = root.get();
    while(cur != nullptr) {
        path.push_back(cur);
        T cur_value = cur->value;
        if(value == cur_value) {
            return iterator(path, this);
        }
        int id = (value < cur_value) ? 0 : 1;
        cur = cur->children[id].get();
    }
    return end();
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::copy(iterator it) {
    std::vector<P<node_t>> path;
    for(node_t* node : it.path) {
        P<node_t> temp(new node_t(*node));
        path.push_back(temp);
    }
    for(size_t i = 0; i + 1 < path.size(); ++i) {
        node_t* node = path[i].get();
        node_t* next_node = it.path[i + 1];
        size_t id = (node->children[0].get() == next_node) ? 0 : 1;
        node->children[id] = path[i + 1];
    }
    root = path[0];
    return iterator(path, this);
}

template <typename T, template <class> class P>
std::pair<typename persistent_set<T, P>::iterator, bool> persistent_set<T, P>::insert(const T& value) {
    iterator it = find(value);
    if(it != end()) {
        return {it, false};
    }

    node_t* cur = root.get();
    if(cur == nullptr) {
        root = P<node_t>(new node_t(value));
        return {iterator(std::vector<node_t*>({root.get()}), this), true};
    }

    root = P<node_t>(new node_t(*root.get()));
    cur = root.get();
    std::vector<node_t*> path;
    while(true) {
        path.push_back(cur);
        int id = (cur->value > value) ? 0 : 1;
        node_t* child = cur->children[id].get();
        if(child == nullptr) {
            cur->children[id] = P<node_t>(new node_t(value));
            path.push_back(cur->children[id].get());
            return {iterator(path, this), true};
        } else {
            cur->children[id] = P<node_t>(new node_t(*child));
            cur = cur->children[id].get();
        }
    }
}

template <typename T, template <class> class P>
void persistent_set<T, P>::erase(iterator __it) {
    if(__it.path.back()->children[1].get() == nullptr) {
        iterator it = copy(__it);

        node_t* node = it.path.back();
        if(node == root.get()) {
            root = node->children[0];
        } else {
            node_t* parent = it.path[it.path.size() - 2];
            int id = (parent->children[0].get() == node) ? 0 : 1;
            parent->children[id] = node->children[0];
        }
    } else {
        iterator __jt = __it;
        __jt++;

        iterator jt = copy(__jt);
        iterator it = iterator(std::vector<node_t*>(jt.path.begin(), jt.path.begin() + __it.path.size()), this);

        node_t* node = it.path.back();
        node_t* substituted = jt.path.back();
        substituted->children[0] = node->children[0];

        P<node_t> temp;

        if(it.path.size() + 1 != jt.path.size()) {
            node_t* substituted_parent = jt.path[jt.path.size() - 2];
            temp = substituted_parent->children[0];
            substituted_parent->children[0] = substituted->children[1];
            substituted->children[1] = node->children[1];
        } else {
            temp = node->children[1];
        }

        if(root.get() == node) {
            root = temp;
        } else {
            node_t* parent = it.path[it.path.size() - 2];
            int id = (parent->children[0].get() == node) ? 0 : 1;
            parent->children[id] = temp;

        }

    }
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::begin() const {
    node_t* cur = root.get();
    if(cur == nullptr) {
        return end();
    }

    std::vector<node_t*> path;
    while(cur != nullptr) {
        path.push_back(cur);
        cur = cur->children[0].get();
    }

    return iterator(path, const_cast<persistent_set<T>*>(this));
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::end() const {
    return iterator(std::vector<node_t*>(), const_cast<persistent_set<T>*>(this));
}

template <typename T, template <class> class P>
persistent_set<T, P>::iterator::iterator(const std::vector<node_t*>& path, persistent_set<T, P>* owner)
    : path(path)
    , owner(owner)
{}

template <typename T, template <class> class P>
persistent_set<T, P>::iterator::iterator(const std::vector<P<node_t>>& nodes, persistent_set<T, P>* owner)
    : path()
    , owner(owner) {
    for(const P<node_t>& node : nodes)
        path.push_back(node.get());
}

template <typename T, template <class> class P>
T const& persistent_set<T, P>::iterator::operator*() const {
    return path.back()->value;
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator& persistent_set<T, P>::iterator::operator++() {
    node_t* cur = path.back();
    if(cur->children[1].get() != nullptr) {
        cur = cur->children[1].get();
        while(cur != nullptr) {
            path.push_back(cur);
            cur = cur->children[0].get();
        }
    } else {
        node_t* prev = cur;
        path.pop_back();
        while(!path.empty()) {
            node_t* cur = path.back();
            if(prev == cur->children[0].get()) {
                break;
            }
            prev = cur;
            path.pop_back();
        }
    }
    return *this;
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::iterator::operator++(int) {
    iterator result(*this);
    ++(*this);
    return result;
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator& persistent_set<T, P>::iterator::operator--() {
    // end() case
    if(path.empty()) {
        node_t* cur = owner->root.get();
        while(cur != nullptr) {
            path.push_back(cur);
            cur = cur->children[1].get();
        }
        return *this;
    }

    node_t* cur = path.back();
    if(cur->children[0].get() != nullptr) {
        cur = cur->children[0].get();
        while(cur != nullptr) {
            path.push_back(cur);
            cur = cur->children[1].get();
        }
    } else {
        node_t* prev = cur;

        while(true) {
            path.pop_back();
            cur = path.back();
            if(cur->children[1].get() == prev) {
                break;
            }
            prev = cur;
        }
    }
    return *this;
}

template <typename T, template <class> class P>
typename persistent_set<T, P>::iterator persistent_set<T, P>::iterator::operator--(int) {
    iterator result(*this);
    --(*this);
    return result;
}

template <typename T, template <class> class P>
bool persistent_set<T, P>::iterator::operator==(const persistent_set<T, P>::iterator& other) const {
    if(path.empty() || other.path.empty()) {
        return path.empty() && other.path.empty();
    }
    return path.back() == other.path.back();
}

template <typename T, template <class> class P>
bool persistent_set<T, P>::iterator::operator!=(const persistent_set<T, P>::iterator& other) {
    return !(*this == other);
}

template <typename T, template <class> class P>
persistent_set<T, P>::node_t::node_t(const T& value)
    : value(value)
    , children({nullptr, nullptr})
{}

#endif //PERSISTENT_SET_H
