#include <any>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

namespace ostr {

template <typename T>
inline void out(std::ostream &os, T const &t) {
  os << t;
}

inline void out(std::ostream &os, const char *a) { os << std::quoted(a); }

inline void out(std::ostream &os, std::string const &s) {
  os << std::quoted(s);
}

inline void out(std::ostream &os, bool b) { os << (b ? "true" : "false"); }

inline void out(std::ostream &os, char c) { os << '\'' << c << '\''; }

template <typename T>
inline void print_vector(std::ostream &os, std::vector<T> const &v) {
  os << '[';
  if (!v.empty()) {
    std::for_each(v.begin(), v.end() - 1, [&](auto const &arg) {
      out(os, arg);
      os << ", ";
    });
    out(os, v.back());
  }
  os << ']';
}

template <typename T>
inline void to_stream(std::ostream &os, T t) {
  out(os, t);
}

template <typename T>
inline void to_stream(std::ostream &os, std::vector<T> v) {
  print_vector(os, v);
}

template <typename T, typename U, typename... Args>
inline void to_stream(std::ostream &os, T t, U u, Args... args) {
  out(os, t);
  os << ", ";
  to_stream(os, u, args...);
}

template <typename T, typename U, typename... Args>
inline void to_stream(std::ostream &os, std::vector<T> v, U u, Args... args) {
  print_vector(os, v);
  to_stream(os, u, args...);
}

struct StreamProxy {
  std::function<std::ostream &(std::ostream &)> op;
  template <typename... Args>
  StreamProxy(Args &&... args)
      : op{[&args...](std::ostream &os) -> std::ostream & {
          to_stream(os, args...);
          return os;
        }} {}
};

inline std::ostream &operator<<(std::ostream &os, StreamProxy const &str) {
  return os.good() ? str.op(os) : os;
}

template <typename... Args>
inline StreamProxy fmt(Args &&... args) {
  return {std::forward<Args>(args)...};
}

}  // namespace ostr
