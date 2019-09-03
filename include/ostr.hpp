#include <functional>
#include <iostream>

struct StreamProxy {
  std::function<std::ostream &(std::ostream &)> op;
  template <typename... Args>
  StreamProxy(Args &&... args)
      : op{[&args...](std::ostream &os) -> std::ostream & {
          auto s = sizeof...(Args);
          unsigned int i = 1;
          ((os << std::forward<Args>(args) << (i++ < s ? ", " : "")), ...);
          return os;
        }} {}
};

std::ostream &operator<<(std::ostream &os, StreamProxy const &str) {
  return os.good() ? str.op(os) : os;
}

template <typename... Args>
StreamProxy print(Args &&... args) {
  return {std::forward<Args>(args)...};
}
