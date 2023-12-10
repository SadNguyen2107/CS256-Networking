#include "Node.h"
#include "../Group.h"

template <typename Value>
Node<Value>::Node(Value data) : data(data), next(nullptr) {}

template <typename Value>
Node<Value>::~Node() {}

// Explicit instantiation of the template class for the types you intend to use
template class Node<Student*>;