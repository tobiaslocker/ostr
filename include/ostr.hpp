#include <any>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

std::ostream &format_any(std::ostream &os, std::any const &value) {
  if (auto s = std::any_cast<bool>(&value)) {
    os << (*s ? "true" : "false");
  } else if (auto s = std::any_cast<int>(&value)) {
    os << *s;
  } else if (auto s = std::any_cast<uint8_t>(&value)) {
    os << (*s & 0xf);
  } else if (auto s = std::any_cast<unsigned short>(&value)) {
    os << *s;
  } else if (auto s = std::any_cast<double>(&value)) {
    os << *s;
  } else if (auto s = std::any_cast<std::string>(&value)) {
    os << std::quoted(*s);
  } else if (auto s = std::any_cast<const char *>(&value)) {
    os << std::quoted(*s);
  } else if (auto s = std::any_cast<char>(&value)) {
    os << '\'' << *s << '\'';
  } else if (auto v = std::any_cast<std::vector<int>>(&value)) {
    os << "[";
    if (!v->empty()) {
      std::for_each(v->begin(), v->end() - 1, [&](auto const &arg) {
        format_any(os, arg) << ", ";
      });
      format_any(os, v->back());
    }
    os << "]";
  } else {
    os << value.type().name() << "Moep";
  }
  return os;
}

struct StreamProxy {
  std::function<std::ostream &(std::ostream &)> op;
  template <typename... Args>
  StreamProxy(Args &&... args)
      : op{[&args...](std::ostream &os) -> std::ostream & {
          std::vector<std::any> v = {args...};
          if (!v.empty()) {
            std::for_each(v.begin(), v.end() - 1, [&](auto const &arg) {
              format_any(os, arg) << ", ";
            });
            format_any(os, v.back());
          }
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
