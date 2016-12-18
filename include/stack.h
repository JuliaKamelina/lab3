#ifndef  _STACK_H_
#define _STACK_H_

#include <stdexcept>

template <typename StackType>

struct StackNode {
  StackType data;
  StackNode * next;
};


template <typename StackType>
class stack {
  StackNode<StackType> *top;
  int count;
 public:
  stack() {
    top = 0;
    count  = 0;
  }
  int getCount();
  bool empty();
  void push(StackType);
  void pop();
  StackType & getTop();
  ~stack();
};


template <typename StackType>
void stack<StackType>::push(StackType x) {
  StackNode<StackType> *node = new StackNode<StackType>;
  
  node->data = x;
  node->next = top;
  top = node;
  count++;
}

template <typename StackType>
void stack<StackType>::pop() {
  if (top != 0) {
    StackNode<StackType> *node = top;
    top = top->next;
    delete node;
    count--;
  } else
    throw std::logic_error( "stack is empty" );
}

template <typename StackType>
StackType & stack<StackType>::getTop() {
  if (top != 0)
    return top->data;
  else
    throw std::logic_error( "stack is empty" );
}

template <typename StackType>
int stack<StackType>::getCount() {
  return count;
}

template <typename StackType>
stack<StackType>::~stack() {
  while (top != 0) {
    StackNode<StackType> *node = top;
    top = top->next;
    delete node;
  }
  count = 0;
}

template <typename StackType>
bool stack<StackType>::empty() {
  return count == 0;
}

#endif  // _STACK_H_