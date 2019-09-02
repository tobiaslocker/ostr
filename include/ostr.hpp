#include <functional>
#include <iostream>

struct StreamProxy {
  std::function<std::ostream &(std::ostream &)> op;

  template <typename... As>
  StreamProxy(As &&... as)
      : op{[&as...](std::ostream &os) -> std::ostream & {
          auto s = sizeof...(As);
          int i = 1;
          ((os << std::forward<As>(as) << (i++ < s ? ", " : "")), ...);
          return os;
        }} {}
};

std::ostream &operator<<(std::ostream &os, StreamProxy const &str) {
  return os.good() ? str.op(os) : os;
}

template <typename... As>
StreamProxy print(As &&... as) {
  return {std::forward<As>(as)...};
}
