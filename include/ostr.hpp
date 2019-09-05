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

template <typename T>
void print_vector(std::ostream &out, std::vector<T> const &v) {
  out << '[';
  if (!v.empty()) {
    std::for_each(
        v.begin(), v.end() - 1, [&](auto const &arg) { out << arg << ", "; });
    out << v.back();
  }
  out << ']';
}

template <typename T>
void to_stream(std::ostream &out, T t) {
  out << t;
}

template <typename T>
void to_stream(std::ostream &out, std::vector<T> v) {
  print_vector(out, v);
}

template <typename T, typename U, typename... Args>
void to_stream(std::ostream &out, T t, U u, Args... args) {
          std::cout << __PRETTY_FUNCTION__ << '\n';
  out << t << ", ";
  to_stream(out, u, args...);
}

template <typename T, typename U, typename... Args>
void to_stream(std::ostream &out, std::vector<T> v, U u, Args... args) {
  print_vector(out, v);
  to_stream(out, u, args...);
}

template <typename U, typename... Args>
void to_stream(std::ostream &out, std::string s, U u, Args... args) {
//          std::cout << __PRETTY_FUNCTION__ << '\n';
  out << std::quoted(s) << ", ";
  to_stream(out, u, args...);
}

template <typename U, typename... Args>
void to_stream(std::ostream &out, const char *s, U u, Args... args) {
          std::cout << __PRETTY_FUNCTION__ << '\n';
  out << std::quoted(s) << ", ";
  to_stream(out, u, args...);
}

template <typename U, typename... Args>
void to_stream(std::ostream &out, bool b, U u, Args... args) {
          std::cout << __PRETTY_FUNCTION__ << '\n';
  out << (b ? "true" : "false");
  to_stream(out, u, args...);
}

struct StreamProxy {
  std::function<std::ostream &(std::ostream &)> op;
  template <typename... Args>
  StreamProxy(Args &&... args)
      : op{[&args...](std::ostream &os) -> std::ostream & {
//          std::cout << __PRETTY_FUNCTION__ << '\n';
          to_stream(os, args...);
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
