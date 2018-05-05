#ifndef LIST_H
#define LIST_H

#include <memory>
#include <iostream>
#include <cassert>
#include <type_traits>
#include <iterator>

template <typename T>
struct list {
private:
    struct iterator_list;
    struct const_iterator_list;

    struct node;
    struct ptr_target;

    node* head;
    node* tail;
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;

    using iterator = list<T>::iterator_list;
    using const_iterator = list<T>::const_iterator_list;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //using const_reference = const T&;

    list():
        head(new node(this)),
        tail(head)
    {}

    list(const list<T> &other): list() {
        for(const_iterator it = other.begin(); it != other.end(); it++)
            push_back(*it);
    }

    list<T>& operator=(const list<T> &other) {
        list<T> new_value(other);
        swap(*this, new_value);
        return *this;
    }

    ~list() {
        node* cur = head;
        while(cur != tail) {
            node* next = cur->next;
            delete cur;
            cur = next;
        }
        delete tail;
    }

    void push_back(const T &value) {
        insert(end(), value);
    }

    void pop_back() {
        erase(--end());
    }

    void push_front(const T &value) {
        insert(begin(), value);
    }

    void pop_front() {
        erase(begin());
    }

    T& front() {
        assert(!empty());
        return *begin();
    }

    const T& front() const {
        assert(!empty());
        return *begin();
    }

    T& back() {
        assert(!empty());
        return *(--end());
    }

    const T& back() const {
        assert(!empty());
        return *(--end());
    }

    iterator erase(iterator left, iterator right) {
        assert(left.valid());
        assert(right.valid());
        assert(left.get_node() != right.get_node());

        list<T> tmp;
        tmp.splice(tmp.begin(), *this, left, right);

        return right;
    }

    iterator erase(iterator pos) {
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
        return iterator(next);
    }

    const_iterator erase(const_iterator pos) {
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
        return const_iterator(next);
    }

    iterator insert(iterator pos, T const& value) {
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

        return iterator(add);
    }

    const_iterator insert(const_iterator pos, T const& value) {
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

        return const_iterator(add);
    }

    void splice(const_iterator pos, list<T> &other, const_iterator first, const_iterator last) {
        assert(pos.valid());
        assert(first.valid());
        assert(last.valid());
        //assert(&other != this);
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
            assert(i != pos.get_node());
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

    iterator begin() {
        return iterator(head);
    }

    const_iterator begin() const {
        return const_iterator(head);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    iterator end() {
        return iterator(tail);
    }

    const_iterator end() const {
        return const_iterator(tail);
    }

    bool empty() const {
        return head == tail;
    }

    friend void swap(list<T> &a, list<T> &b) {
        list<T>::node* cur = a.head;
        while(cur != a.tail) {
            cur -> owner = &b;
            cur = cur -> next;
        }
        a.tail -> owner = &b;

        cur = b.head;
        while(cur != b.tail) {
            cur -> owner = &a;
            cur = cur -> next;
        }
        b.tail -> owner = &a;

        std::swap(a.head, b.head);
        std::swap(a.tail, b.tail);
    }

};

template <typename T>
struct list<T>::iterator_list {
private:
    std::shared_ptr<ptr_target> ptr;

    iterator_list(node* x):
        ptr(x->ptr)
    {}

    bool valid() const {
        return ptr->valid;
    }

    list<T>* owner() const {
        return ptr->node_ptr->owner;
    }

    list<T>::node* get_node() const {
        return ptr->node_ptr;
    }

    friend struct list<T>;
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_list(const list<T>::iterator_list &other): ptr(other.ptr)
    {}

    iterator_list():
        ptr(new ptr_target)
    {
        ptr.get() -> valid = false;
    }

    T& operator*() const {
        assert(valid());
        assert(ptr->node_ptr != owner()->tail);
        return ptr->node_ptr->value;
    }

    iterator_list& operator++() {
        assert(valid());
        assert(get_node() != owner()->tail);
        ptr = get_node()->next->ptr;
        return *this;
    }

    iterator_list operator++(int) {
        assert(valid());
        assert(get_node() != owner()->tail);
        iterator_list result = *this;
        ptr = get_node()->next->ptr;
        return result;
    }

    iterator_list& operator--() {
        assert(valid());
        assert(get_node() != owner()->head);
        ptr = get_node()->prev->ptr;
        return *this;
    }

    iterator_list operator--(int) {
        assert(valid());
        assert(get_node() != owner()->head);
        iterator_list result = *this;
        ptr = get_node()->prev->ptr;
        return result;
    }

    friend bool operator==(const list<T>::iterator &a, const list<T>::iterator &b) {
        assert(a.valid() && b.valid());
        assert(b.owner() == a.owner());
        return a.ptr == b.ptr;
    }

    friend bool operator!=(const list<T>::iterator &a, const list<T>::iterator &b) {
        assert(a.valid() && b.valid());
        assert(b.owner() == a.owner());
        return a.ptr != b.ptr;
    }
};

template <typename T>
struct list<T>::const_iterator_list {
private:
    std::shared_ptr<ptr_target> ptr;

    const_iterator_list(node* x):
        ptr(x->ptr)
    {}

    bool valid() const {
        return ptr->valid;
    }

    list<T>* owner() const {
        return ptr->node_ptr->owner;
    }

    list<T>::node* get_node() const {
        return ptr->node_ptr;
    }

    friend struct list<T>;

public:
    using difference_type = std::ptrdiff_t;
    using value_type = T const;
    using pointer = T const*;
    using reference = T const&;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator_list(const list<T>::iterator_list &other): ptr(other.ptr)
    {}

    const_iterator_list(const list<T>::const_iterator_list &other): ptr(other.ptr)
    {}

    const_iterator_list():
        ptr(new ptr_target)
    {
        ptr.get() -> valid = false;
    }

    T const& operator*() const {
        assert(valid());
        assert(ptr->node_ptr != owner()->tail);
        return ptr->node_ptr->value;
    }

    const_iterator_list& operator++() {
        assert(valid());
        assert(get_node() != owner()->tail);
        ptr = get_node()->next->ptr;
        return *this;
    }

    const_iterator_list operator++(int) {
        assert(valid());
        assert(get_node() != owner()->tail);
        const_iterator_list result = *this;
        ptr = get_node()->next->ptr;
        return result;
    }

    const_iterator_list& operator--() {
        assert(valid());
        assert(get_node() != owner()->head);
        ptr = get_node()->prev->ptr;
        return *this;
    }

    const_iterator_list operator--(int) {
        assert(valid());
        assert(get_node() != owner()->head);
        const_iterator_list result = *this;
        ptr = get_node()->prev->ptr;
        return result;
    }

    friend bool operator==(const list<T>::const_iterator &a, const list<T>::const_iterator &b) {
        assert(a.valid() && b.valid());
        assert(b.owner() == a.owner());
        return a.ptr == b.ptr;
    }

    friend bool operator!=(const list<T>::const_iterator &a, const list<T>::const_iterator &b) {
        assert(a.valid() && b.valid());
        assert(b.owner() == a.owner());
        return a.ptr != b.ptr;
    }
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
    ptr_target()
    {}

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



#endif // LIST_H
