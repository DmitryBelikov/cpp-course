#ifndef LIST_H
#define LIST_H

#include <memory>
#include <iostream>
#include <cassert>
#include <type_traits>
#include <iterator>

// ptr target with no ctor - for default iterator
// swap need compile
// smth strange 23

template <typename T>
struct list {
public:
    struct iterator_list;

    using value_type = T;
    using difference_type = std::ptrdiff_t;

    using iterator = list<T>::iterator_list;
    using const_iterator = const list<T>::iterator_list&;????
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list();
    list(list& other);
    list& operator=(const list& other);

    ~list();

    void push_back(T const&);
    void pop_back();
    void push_front(T const&);
    void pop_front();

    iterator_list erase(iterator_list);
    iterator_list insert(iterator_list, T const&);

    T& front();
    T const& front() const;

    T& back();
    T const& back() const;

    void splice(iterator_list const& pos, list<T>& other,
                iterator_list const& first, iterator_list const& last);

    list<T>::iterator_list begin();
    list<T>::const_iterator begin() const;
    list<T>::iterator_list end();
    list<T>::const_iterator end() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    bool empty() const;

    friend class list<T>::iterator_list;
    friend void swap(list &a, list &b) {
        list<T>::node* cur = a.head;
        while(cur != a.tail)
            cur -> owner = *b;
        a.tail -> owner = *b;

        while(cur != b.tail)
            cur -> owner = *a;
        b.tail -> owner = *a;

        std::swap(a.head, b.head);
        std::swap(a.tail, b.tail);
    }

private:
    struct node;
    struct ptr_target;

    node* head;
    node* tail;
};

template <typename T>
struct list<T>::iterator_list {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_list():
        ptr(new ptr_target)
    {
        ptr.get() -> valid = false;
    }

    T& operator*() const;

    iterator_list& operator++();
    iterator_list operator++(int);
    iterator_list& operator--();
    iterator_list operator--(int);

    friend bool operator==(const typename list<T>::iterator_list &a, const typename list<T>::iterator_list &b) {
        assert(a.valid() && b.valid());
        assert(b.owner() == a.owner());
        return a.ptr == b.ptr;
    }

    friend bool operator!=(const typename list<T>::iterator_list &a, const typename list<T>::iterator_list &b) {
        assert(a.valid() && b.valid());
        assert(b.owner() == a.owner());
        return a.ptr != b.ptr;
    }

    bool operator==(const typename list<T>::iterator_list &other) {
        assert(valid() && other.valid());
        assert(other.owner() == this->owner());
        return ptr == other.ptr;
    }

    bool operator!=(const typename list<T>::iterator_list &other) {
        assert(valid() && other.valid());
        assert(other.owner() == this->owner());
        return ptr != other.ptr;
    }



private:
    std::shared_ptr<ptr_target> ptr;
    iterator_list(node*);

    bool valid() const;
    list<T>* owner() const;
    list<T>::node* get_node() const;

    friend struct list<T>;
};

template <typename T>
struct list<T>::node {
public:
    node(list<T>* const);
    node(T const&, list<T>* const);
    ~node();

    T value;
    node* next;
    node* prev;

    list<T>* owner;
    std::shared_ptr<ptr_target> ptr;
};

template <typename T>
struct list<T>::ptr_target {
public:
    ptr_target(list<T>::node* x);

    bool valid;
    typename list<T>::node* node_ptr;
};

template <typename T>
list<T>::ptr_target::ptr_target(list<T>::node* x):
    valid(true),
    node_ptr(x)
{}

template <typename T>
list<T>::node::node(list<T>* const list):
    next(nullptr),
    prev(nullptr),
    owner(list),
    ptr(new ptr_target(this))
{}

template <typename T>
list<T>::node::node(T const& value, list<T>* const list):
    value(value),
    next(nullptr),
    prev(nullptr),
    owner(list),
    ptr(new ptr_target(this))
{}

template <typename T>
list<T>::node::~node() {
    ptr->valid = false;
}


template <typename T>
T& list<T>::iterator_list::operator*() const {
    assert(valid());
    assert(ptr->node_ptr != owner()->tail);
    return ptr->node_ptr->value;
}

template <typename T>
typename list<T>::iterator_list& list<T>::iterator_list::operator++() {
    assert(valid());
    assert(get_node() != owner()->tail);
    ptr = get_node()->next->ptr;
    return *this;
}

template <typename T>
typename list<T>::iterator_list list<T>::iterator_list::operator++(int) {
    assert(valid());
    assert(get_node() != owner()->tail);
    const_iterator result = *this;
    ptr = get_node()->next->ptr;
    return result;
}

template <typename T>
typename list<T>::iterator_list& list<T>::iterator_list::operator--() {
    assert(valid());
    assert(get_node() != owner()->head);
    ptr = get_node()->prev->ptr;
    return *this;
}

template <typename T>
typename list<T>::iterator_list list<T>::iterator_list::operator--(int) {
    assert(valid());
    assert(get_node() != owner()->head);
    const_iterator result = *this;
    ptr = get_node()->prev->ptr;
    return result;
}

template <typename T>
list<T>::iterator_list::iterator_list(node* x):
    ptr(x->ptr)
{}

template <typename T>
bool list<T>::iterator_list::valid() const {
    return ptr->valid;
}

template <typename T>
list<T>* list<T>::iterator_list::owner() const {
    return ptr->node_ptr->owner;
}

template <typename T>
typename list<T>::node* list<T>::iterator_list::get_node() const {
    return ptr->node_ptr;
}

template <typename T>
list<T>::list():
    head(new node(this)),
    tail(head)
{}

template <typename T>
list<T>::list(list &other): list() {
    for(auto it = other.begin(); it != other.end(); it++)
        push_back(*it);
}

template <typename T>
list<T>& list<T>::operator=(const list &other) {
    while(!empty())
        pop_back();
    for(auto it = other.begin(); it != other.end(); it++)
        push_back(*it);
    return *this;
}

template <typename T>
list<T>::~list() {
    node* cur = head;
    while(cur != tail) {
        node* next = cur->next;
        delete cur;
        cur = next;
    }
    delete tail;
}

template <typename T>
void list<T>::push_back(const T &value) {
    insert(end(), value);
}

template <typename T>
void list<T>::pop_back() {
    erase(--end());
}

template <typename T>
void list<T>::push_front(const T &value) {
    insert(begin(), value);
}

template <typename T>
void list<T>::pop_front() {
    erase(begin());
}

template <typename T>
T& list<T>::front() {
    assert(!empty());
    return *begin();
}

template <typename T>
T const& list<T>::front() const {
    assert(!empty());
    return *begin();
}

template <typename T>
T& list<T>::back() {
    assert(!empty());
    return *(--end());
}

template <typename T>
T const& list<T>::back() const {
    assert(!empty());
    return *(--end());
}

template <typename T>
typename list<T>::iterator_list list<T>::erase(iterator_list pos) {
    assert(pos.valid());
    assert(pos.owner() == this);
    assert(tail != pos.get_node());

    node* mid = pos.get_node();
    node* next = mid->next;
    node* prev = mid->prev;

    if(next != nullptr) {
        next->prev = prev;
    }

    if(prev != nullptr) {
        prev->next = next;
    }

    if(mid->owner->head == mid)
        head = mid->next;

    delete mid;
    return iterator_list(next);
}

template <typename T>
typename list<T>::iterator_list list<T>::insert(iterator_list pos, T const& value) {
    assert(pos.valid());
    assert(pos.owner() == this);
    node* add = new node(value, this);
    node* cur = pos.get_node();

    add->prev = cur->prev;
    add->next = cur;

    if(cur != head) {
        cur->prev->next = add;
    } else {
        head = add;
    }

    cur->prev = add;

    return iterator_list(add);
}

template <typename T>
void list<T>::splice(const iterator_list &pos, list<T> &other, const iterator_list &first, const iterator_list &last) {
    assert(pos.valid());
    assert(first.valid());
    assert(last.valid());
    assert(&other != this);
    assert(first.owner() == &other);
    assert(last.owner() == &other);
    assert(pos.owner() == this);

    if(first.get_node() == last.get_node())
        return;

    node* cur = pos.get_node();

    node* first_ptr = first.get_node();
    node* other_prev = first_ptr->prev;
    node* other_next = last.get_node();
    node* other_last = other_next->prev;

    if(first_ptr == first_ptr->owner->head)
        first_ptr->owner->head = other_next;

    node* i = first_ptr;
    while(i != other_next) {
        assert(i != i->owner->tail);
        i->owner = this;
        i = i->next;
    }



    if(other_prev != nullptr) {
        other_prev->next = other_next;
    } else {
        other.head = other_next;
    }

    other_next->prev = other_prev;

    if(cur != head) {
        first_ptr->prev = cur->prev;
    } else {
        head = first_ptr;
        first_ptr->prev = nullptr;
    }

    other_last->next = cur;

    if(cur->prev != nullptr) {
        cur->prev->next = first_ptr;
    }

    cur->prev = other_last;
}

template <typename T>
bool list<T>::empty() const {
    return head == tail;
}

template <typename T>
typename list<T>::iterator_list list<T>::begin() {
    return iterator_list(head);
}

template <typename T>
typename list<T>::const_iterator list<T>::begin() const {
    return const_iterator(head);
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rbegin() const {
    return reverse_iterator(end());
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rend() const {
    return reverse_iterator(begin());
}

template <typename T>
typename list<T>::iterator_list list<T>::end() {
    return iterator_list(tail);
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const {
    return const_iterator(tail);
}

#endif // LIST_H
