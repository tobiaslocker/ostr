#include <any>
#include <functional>
#include <iostream>
#include <vector>

struct StreamProxy {
  std::function<std::ostream &(std::ostream &)> op;
  template <typename... Args>
  StreamProxy(Args &&... args)
      : op{[&args...](std::ostream &os) -> std::ostream & {
          std::vector<std::any> v = {args...};
          std::for_each(v.begin(), v.end() - 1, [&](auto arg) {
            os << arg.type().name() << ", ";
          });
          os << v.back().type().name();
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
