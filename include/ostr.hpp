#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class Manipulator {
  std::function<std::ostream &(std::ostream &, T)> m_func;
  T m_arg;

 public:
  Manipulator(std::function<std::ostream &(std::ostream &, T)> f, T arg)
      : m_func(f), m_arg(arg) {}
  inline void do_op(std::ostream &str) const { m_func(str, m_arg); }
};

template <typename T>
class PrintVector : public Manipulator<std::vector<T> const &> {
  inline static std::ostream &call(std::ostream &os, std::vector<T> const &v) {
    os << "{ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
    return os << "}";
  }

 public:
  inline PrintVector(std::vector<T> const &v)
      : Manipulator<std::vector<T> const &>(call, v) {}
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, Manipulator<T> const &m) {
  if (!os.good()) return os;
  m.do_op(os);
  return os;
}

template <typename T>
class Print : public Manipulator<T const &> {
  inline static std::ostream &call(std::ostream &os, T const &v) {
    os << v;
    return os;
  }

 public:
  inline Print(T const &v)
      : Manipulator<T const &>(call, v) {}
};

template <class T>
inline PrintVector<T> print(std::vector<T> const &v) {
  return PrintVector<T>(v);
}

template <class T>
inline Print<T> print(T const &v) {
  return Print<T>(v);
}
