#include <iostream>
#include <string>
typedef struct Node {
  int min;
  int val;
  struct Node* next;
} Node;
class Stack {
 private:
  Node *val_;
  int size_;
  
 public:
  Stack() {
    val_ = nullptr;
    size_ = 0;
  }
  void Push(int value) {
    Node* new_val = new Node;
    int min = value;
    if (size_ > 0) {
      if (min > val_->min) {
        min = val_->min;
      }
      new_val->val = value;
      new_val->min = min;
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
    Node *prev = val_;
    val_ = prev->next;
    delete prev;
    size_--;
    return result;
  }
  void Min() {
    if (val_ == nullptr || size_ == 0) {
      std::cout << "error" << std::endl;
      return;
    }
    std::cout << val_->min << std::endl;
  }
  void Back() {
    if (val_ == nullptr || size_ == 0) {
      std::cout << "error" << std::endl;
      return;
    }
    std::cout << val_->val << std::endl;
  }
  int Size() const {
    return size_;
  }
  void Clear() {
    while (size_ > 0) {
      Pop();
    }
  }
  ~Stack() {
    Clear();
  }
};

int main() {
  Stack st = Stack();
  int count = 0;
  std::cin >> count;
  std::string str;
  while (count > 0) {
    std::cin >> str;
    if (str == "push") {
      int value;
      std::cin >> value;
      st.Push(value);
      std::cout << "ok" << std::endl;
    } else if (str == "back") {
      st.Back();
    } else if (str == "pop") {
      try {
        std::cout << st.Pop() << std::endl;
      } catch (std::invalid_argument const &ex) {
        std::cout << ex.what() << std::endl;
      }
    } else if (str == "min") {
      st.Min();
    } else if (str == "size") {
      std::cout << st.Size() << std::endl;
    } else if (str == "clear") {
      st.Clear();
      std::cout << "ok" << std::endl;
    }
    --count;
  }
}