/* Copyright (C) 2015 Ben Lewis <benjf5+github@gmail.com>
 * Licensed under the MIT license.
 *
 * Test code for the Queue class. Compile with any C++ compiler to test the
 * Queue's functionality.
 */

#include "round_robin_queue.h"
#include <iostream>

int main()
{
  Queue<int, 4> test_queue;

  if (test_queue.enqueue(1))
  {
    std::cout << "PASSED: first enqueue succeeded" << std::endl;
  }

  if (test_queue.enqueue(2))
  {
    std::cout << "PASSED: second enqueue succeeded" << std::endl;
  }

  if (test_queue.enqueue(3))
  {
    std::cout << "PASSED: third enqueue succeeded" << std::endl;
  }

  if (test_queue.enqueue(4))
  {
    std::cout << "PASSED: fourth enqueue succeeded" << std::endl;
  }

  if (!test_queue.enqueue(5))
  {
    std::cout << "PASSED: fifth enqueue failed" << std::endl;
  }

  int retrieved = 0;

  if (test_queue.dequeue(&retrieved))
  {
    std::cout << "PASSED: first dequeue succeeded with value " << retrieved << std::endl;
  }

  if (test_queue.dequeue(&retrieved))
  {
    std::cout << "PASSED: second dequeue with value " << retrieved << std::endl;
  }

  if (test_queue.dequeue(&retrieved))
  {
    std::cout << "PASSED: third dequeue succeeded with value " << retrieved << std::endl;
  }

  if (test_queue.dequeue(&retrieved))
  {
    std::cout << "PASSED: fourth dequeue succeeded with value " << retrieved << std::endl;
  }

  if (!test_queue.dequeue(&retrieved))
  {
    std::cout << "PASSED: fifth dequeue failed with value " << retrieved << std::endl;
  }

  std::cout << "TEST SECTION: Different numbers of enqueues and dequeues" << std::endl;

  if (test_queue.enqueue(1))
  {
    std::cout << "PASSED: first enqueue succeeded" << std::endl;
  }

  if (test_queue.enqueue(2))
  {
    std::cout << "PASSED: second enqueue succeeded" << std::endl;
  }

  if (test_queue.enqueue(3))
  {
    std::cout << "PASSED: third enqueue succeeded" << std::endl;
  }

  if (test_queue.enqueue(4))
  {
    std::cout << "PASSED: fourth enqueue succeeded" << std::endl;
  }

  if (!test_queue.enqueue(5))
  {
    std::cout << "PASSED: fifth enqueue failed" << std::endl;
  }

  if (test_queue.dequeue(&retrieved))
  {
    std::cout << "PASSED: first dequeue succeeded with value " << retrieved << std::endl;
  }

  if (test_queue.enqueue(5))
  {
    std::cout << "PASSED: fifth enqueue succeeded" << std::endl;
  }

  if (!test_queue.enqueue(6))
  {
    std::cout << "PASSED: sixth enqueue failed" << std::endl;
  }

  return 0;
}
