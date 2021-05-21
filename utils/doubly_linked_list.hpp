#include <iostream>

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
