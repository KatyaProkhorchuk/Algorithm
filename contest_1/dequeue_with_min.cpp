#include <iostream>
#include <string>
typedef struct Node {
  int min;
  int val;
  struct Node* next;
} Node;
class Stack {
 public:
  Stack() {
    val_ = nullptr;
    size_ = 0;
  }
  void Push(int value) {
    Node* new_val = new Node;
    if (size_ > 0) {
      int min = value;
      if (min > val_->min) {
        min = val_->min;
      }
      new_val->min = min;
      new_val->val = value;
      new_val->next = this->val_;
      val_ = new_val;
    } else {
      new_val->val = value;
      new_val->min = value;
      new_val->next = nullptr;
      val_ = new_val;
    }

    size_++;
  }
  int Pop() {
    if (val_ == nullptr || size_ == 0) {
      delete val_;
      throw std::invalid_argument("error");
    }
    int result = val_->val;
    Node* prev = val_;
    val_ = prev->next;
    delete prev;
    size_--;
    return result;
  }
  int Min() {
    if (val_ == nullptr || size_ == 0) {
      throw std::invalid_argument("error");
    }
    return val_->min;
  }
  void Back() {
    if (val_ == nullptr || size_ == 0) {
      std::cout << "error" << std::endl;
      return;
    }
    std::cout << val_->val << std::endl;
  }
  int Size() const { return size_; }
  void Clear() {
    while (size_ > 0) {
      Pop();
    }
  }
  ~Stack() { Clear(); }

 private:
  Node* val_;
  int size_;
};

class Queue {
 public:
  void Enqueue(int val) {
    push_.Push(val);
    if (pop_.Size() == 0) {
      while (push_.Size() > 0) {
        pop_.Push(push_.Pop());
      }
    }
    std::cout << "ok" << std::endl;
  }
  void Dequeue() {
    if (pop_.Size() == 0 && push_.Size() == 0) {
      throw std::invalid_argument("error");
    } else if (pop_.Size() == 0 && push_.Size() != 0) {
      while (push_.Size() > 0) {
        pop_.Push(push_.Pop());
      }
    }
    std::cout << pop_.Pop() << std::endl;
  }
  void Front() {
    if (pop_.Size() == 0 && push_.Size() == 0) {
      std::cout << "error" << std::endl;
      return;
    }
    if (pop_.Size() == 0 && push_.Size() != 0) {
      while (push_.Size() > 0) {
        pop_.Push(push_.Pop());
      }
    }
    pop_.Back();
  }
  int Size() { return push_.Size() + pop_.Size(); }
  void Clear() {
    push_.Clear();
    pop_.Clear();
    std::cout << "ok" << std::endl;
  }
  int Min() {
    int min;
    if (pop_.Size() == 0 && push_.Size() == 0) {
      throw std::invalid_argument("error");
    } else if (pop_.Size() == 0 && push_.Size() != 0) {
      min = push_.Min();
    } else if (pop_.Size() != 0 && push_.Size() == 0) {
      min = pop_.Min();
    } else if (pop_.Size() != 0 && push_.Size() != 0) {
      min = push_.Min();
      if (min > pop_.Min()) {
        min = pop_.Min();
      }
    }
    return min;
  }

 private:
  Stack push_, pop_;
};

void Run(Queue* min_queue, int count) {
  std::string str;
  for (int i = 0; i < count; ++i) {
    std::cin >> str;
    if (str == "enqueue") {
      int value;
      std::cin >> value;
      min_queue->Enqueue(value);
    } else if (str == "dequeue") {
      try {
        min_queue->Dequeue();
      } catch (std::invalid_argument const& ex) {
        std::cout << ex.what() << std::endl;
      }
    } else if (str == "front") {
      min_queue->Front();
    } else if (str == "size") {
      std::cout << min_queue->Size() << std::endl;
    } else if (str == "clear") {
      min_queue->Clear();
    } else if (str == "min") {
      try {
        std::cout << min_queue->Min() << std::endl;
      } catch (std::invalid_argument const& ex) {
        std::cout << ex.what() << std::endl;
      }
    }
  }
}

int main() {
  Queue min_queue = Queue();
  int count = 0;
  std::cin >> count;
  Run(&min_queue, count);
}