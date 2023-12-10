#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <functional>
#include <iostream>

template <typename Value>
class LinkedList
{
private:
    Node<Value> *head;

public:
    LinkedList();
    ~LinkedList();

    // Return the Head Node of the LinkedList
    Node<Value> *getHead();

    // Add a new node with given data to the end of the linked list
    void add(Value value_to_add);

    // Remove the node with value to delete
    void remove(std::function<bool(Value, Value)> isTrue, Value value_to_compare);

    //* DEBUG PURPOSE
    friend std::ostream &operator<<(std::ostream &os, const LinkedList<Value> &linkedList)
    {
        Node<Value> *current_head = linkedList.head;

        int pos = 1;
        while (current_head != nullptr)
        {
            os << "[" << pos << "] " << current_head->data << '\n';
            current_head = current_head->next;
            pos++;
        }
        os << std::endl;

        return os;
    }
};

#endif