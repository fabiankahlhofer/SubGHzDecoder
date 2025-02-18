#include <iostream>
#include <sstream>

template <typename T>
class Queue {
 private:
  struct Node {
    T data;
    Node *next;
    Node(T newData) : data(newData), next(nullptr) {}
  };

  Node *frontPtr;
  Node *rearPtr;
  int size;

 public:
  Queue() : frontPtr(nullptr), rearPtr(nullptr), size(0) {}

  ~Queue() {
    while (!isEmpty()) {
      dequeue();
    }
  }

  void enqueue(const T &newData) {
    Node *newNode = new Node(newData);
    if (isEmpty()) {
      frontPtr = rearPtr = newNode;
    } else {
      rearPtr->next = newNode;
      rearPtr = newNode;
    }
    size++;
  }

  void dequeue() {
    if (!isEmpty()) {
      Node *temp = frontPtr;
      frontPtr = frontPtr->next;
      delete temp;
      size--;
      if (isEmpty()) {
        rearPtr = nullptr;
      }
    } else {
      std::cerr << "Error: Queue is empty. Cannot dequeue." << std::endl;
    }
  }

  T front() const {
    if (!isEmpty()) {
      return frontPtr->data;
    } else {
      std::cerr << "Error: Queue is empty. No front element." << std::endl;
      return T();  // Return default-constructed object
    }
  }

  bool isEmpty() const { return size == 0; }

  int getSize() const { return size; }

  std::string toString() const {
    std::ostringstream oss;
    oss << "Queue: ";
    Node *current = frontPtr;
    while (current != nullptr) {
      oss << current->data << " ";
      current = current->next;
    }
    return oss.str();
  }
};