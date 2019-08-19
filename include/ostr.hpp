#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class StreamProxy {
  std::function<std::ostream &(std::ostream &, T)> m_func;
  T m_arg;

 public:
  StreamProxy(std::function<std::ostream &(std::ostream &, T)> f, T arg)
      : m_func(f), m_arg(arg) {}
  inline void do_op(std::ostream &str) const { m_func(str, m_arg); }
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, StreamProxy<T> const &m) {
  if (!os.good()) return os;
  m.do_op(os);
  return os;
}

template <typename T>
class PrintVector : public StreamProxy<std::vector<T> const &> {
  inline static std::ostream &call(std::ostream &os, std::vector<T> const &v) {
    os << "{ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
    return os << "}";
  }

 public:
  inline PrintVector(std::vector<T> const &v)
      : StreamProxy<std::vector<T> const &>(call, v) {}
};

template <typename T>
class Print : public StreamProxy<T const &> {
  inline static std::ostream &call(std::ostream &os, T const &v) {
    os << v;
    return os;
  }

 public:
  inline Print(T const &v)
      : StreamProxy<T const &>(call, v) {}
};

template <typename T>
inline PrintVector<T> print(std::vector<T> const &v) {
  return PrintVector<T>(v);
}

template <typename T>
inline auto print(T const &v) {
  std::cout << __PRETTY_FUNCTION__ << "\n";
  return Print<T>(v);
}


template <typename T, typename ...Args>
inline auto print(T const &v, Args&& ...args) {
    return print(v) << print(args...);
//  return Print<T>(args...);
}






