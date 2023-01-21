#include <cctype>
#include <iostream>

typedef struct Node {
  int min;
  int val;
  struct Node* next;
} Node;
class Stack {
 public:
  Stack() : val_(nullptr), size_(0) {}
  void Push(int value) {
    Node* new_val = new Node;
    if (val_ != nullptr) {
      int min = value;
      if (min > val_->min) {
        min = val_->min;
      }
      new_val->min = min;
      new_val->val = value;
      new_val->next = val_;
      val_ = new_val;
    } else {
      new_val->val = value;
      new_val->min = value;
      new_val->next = nullptr;
      val_ = new_val;
    }

    ++size_;
  }
  int Pop() {
    if (val_ == nullptr) {
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
    if (val_ == nullptr) {
      throw std::invalid_argument("error");
    }
    return val_->min;
  }
  void Back() {
    if (val_ == nullptr) {
      std::cout << "error" << std::endl;
      return;
    }
    std::cout << val_->val << std::endl;
  }
  int Size() const { return size_; }
  void Clear() {
    while (val_ != nullptr) {
      Pop();
    }
  }
  ~Stack() { Clear(); }

 private:
  Node* val_;
  int size_;
};

void Run(Stack* st, const std::string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (std::isdigit(str[i]) != 0) {
      st->Push(int(str[i]) - 48);
    } else if (str[i] == '+') {
      int first_el = st->Pop();
      int second_el = st->Pop();
      st->Push(first_el + second_el);
    } else if (str[i] == '-') {
      int first_el = st->Pop();
      int second_el = st->Pop();
      st->Push(second_el - first_el);
    } else if (str[i] == '*') {
      int first_el = st->Pop();
      int second_el = st->Pop();
      st->Push(first_el * second_el);
    }
  }
  st->Back();
}

int main() {
  Stack st;
  std::string str;
  getline(std::cin, str);
  Run(&st, str);
}