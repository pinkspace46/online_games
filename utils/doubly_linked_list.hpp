#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include <iterator>

template <typename T>
struct node {
    T data;
    struct node *next;
    struct node *prev;
};

template <typename T>
class doubly_linked_list {
    private:
        node<int> dummy_node;
    public:
        doubly_linked_list();
        void add(T);
        void remove(T);
        void print();

        struct Iterator {
            public:
                using iterator_category = std::bidirectional_iterator_tag;
                using difference_type   = int;
                using value_type        = T;
                using pointer           = node<T>*;
                using reference         = T&;
                Iterator(pointer ptr): _ptr(ptr) {}
                reference operator*() const {return _ptr->data;}
                pointer operator->() {return _ptr;}
                Iterator& operator++() {_ptr = _ptr->next; return *this;}
                Iterator& operator--() {_ptr = _ptr->prev; return *this;}
                Iterator operator++(int) {Iterator tmp = *this; ++(*this); return tmp;}
                Iterator operator--(int) {Iterator tmp = *this; --(*this); return tmp;}
                friend bool operator==(const Iterator& a, const Iterator& b) {return a._ptr == b._ptr;};
                friend bool operator!=(const Iterator& a, const Iterator& b) {return a._ptr != b._ptr;};
            private:
                pointer _ptr;
        };
        
        Iterator begin() {return Iterator(dummy_node.next);}
        Iterator end() {return Iterator(&dummy_node);}
};

template <typename T>
doubly_linked_list<T>::doubly_linked_list()
{
    dummy_node.data = -1;
    dummy_node.next = &dummy_node;
    dummy_node.prev = &dummy_node;
}

template <typename T>
void doubly_linked_list<T>::add(T data)
{
    struct node<T> *ptr = new struct node<T>;
    ptr->next = dummy_node.next;
    ptr->prev = &dummy_node;
    ptr->data = data;
    dummy_node.next->prev = ptr;
    dummy_node.next = ptr;
}

template <typename T>
void doubly_linked_list<T>::remove(T data)
{
    struct node<T> *ptr = dummy_node.next;
    while (true) {
        if (ptr == &dummy_node) {
            return;
        }
        if (ptr->data == data) {
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
            delete ptr;
            return;
        }
        ptr = ptr->next;
    }
}

template <typename T>
void doubly_linked_list<T>::print()
{
    struct node<T> *ptr = dummy_node.next;
    while (true) {
        if (ptr == &dummy_node) {
            break;
        }
        std::cout << " " << ptr->data;
        ptr = ptr->next;
    }
    std::cout << std::endl;
}

#endif // DOUBLY_LINKED_LIST_H
