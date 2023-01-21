#include <iostream>

typedef struct Node {
  char val;
  struct Node* next;
} Node;
class Stack {
 public:
  Stack() : val_(nullptr), size_(0) {}
  void Push(char value) {
    Node* new_val = new Node;
    new_val->val = value;
    new_val->next = val_;
    val_ = new_val;
    ++size_;
  }
  char Pop() {
    if (val_ == nullptr) {
      throw std::invalid_argument("error");
    }
    char result = val_->val;
    Node* prev = val_;
    val_ = prev->next;
    delete prev;
    size_--;
    return result;
  }
  char Back() {
    if (val_ == nullptr) {
      std::cout << "error" << std::endl;
    }
    return val_->val;
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

std::string Run(Stack &st, const std::string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (str[i] == '[' || str[i] == '(' || str[i] == '{') {
      st.Push(str[i]);
    } else if (str[i] == ')' || str[i] == ']' || str[i] == '}') {
      if (st.Size() != 0) {
        char el = st.Back();
        if ((el == '(' && str[i] == ')') || (el == '[' && str[i] == ']') ||
            (el == '{' && str[i] == '}')) {
          st.Pop();
        } else {
          return "NO";
        }
      } else {
        return "NO";
      }
    }
  }
  if (st.Size() == 0) {
    return "YES";
  }
  return "NO";
}

int main() {
  Stack st;
  std::string str;
  getline(std::cin, str);
  std::cout << Run(st, str) << std::endl;
}