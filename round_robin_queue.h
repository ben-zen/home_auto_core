/* Copyright (C) 2015 Ben Lewis <benjf5+github@gmail.com>
 * Licensed under the MIT license.
 *
 * Defines a round-robin implementation of a Queue using simple enqueue and
 * dequeue operations, and a fixed-size queue space of stack-allocated objects.
 * If an enqueue operation fails, it will return false and set the overflow
 * value to true.
 */

template <typename Element, size_t Size>
class Queue
{
private:
  Element elements[Size];
  size_t start;
  size_t end;
  bool full;

public:
  Queue() : start(0), end(0), full(false)
  {
    elements;
  }

  bool dequeue(Element *out)
  {
    *out = { };
    bool response = false;
    if (start != end || full)
    {
      *out = elements[start];
      elements[start] = { };
      start = (start + 1) % Size;
      full = false;
      response = true;
    }
    return response;
  }

  bool enqueue(Element const &insert)
  {
    bool response = false;
    if (!full)
    {
      elements[end] = insert;
      end = (end + 1) % Size;
      response = true;
      full = (end == start);
    }
    return response;
  }
};
