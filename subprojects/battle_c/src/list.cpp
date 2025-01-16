#include "list.hpp"

LinkedList *LinkedList::Next() const { return next_; }

LinkedList *LinkedList::Prev() const { return prev_; }

void *LinkedList::Value() const { return value_; }

LinkedList *LinkedList::Rewind()
{
  LinkedList *current = this;
  while (current->prev_ != nullptr)
  {
    current = current->prev_;
  }
  return current;
}

LinkedList *LinkedList::WalkToEnd()
{
  LinkedList *current = this;
  while (current->next_ != nullptr)
  {
    current = current->next_;
  }
  return current;
}

void LinkedList::Free(LinkedList *linked_list)
{
  LinkedList *current = linked_list->Rewind();
  while (current != nullptr)
  {
    LinkedList *temp = current;
    current = current->Next();
    delete temp;
  }
}

void LinkedList::Add(LinkedList *next)
{
  if (this != nullptr)
  {
    this->next_ = next;
  }
  if (next != nullptr)
  {
    next->prev_ = this;
  }
}