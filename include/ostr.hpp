#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class manipulator {
  std::function<std::ostream &(std::ostream &, T)> m_func;
  T m_arg;

 public:
  manipulator(std::function<std::ostream &(std::ostream &, T)> f, T arg)
      : m_func(f), m_arg(arg) {}
  void do_op(std::ostream &str) const { m_func(str, m_arg); }
};

template <typename T>
class do_print : public manipulator<std::vector<T> const &> {
  static std::ostream &call(std::ostream &os, std::vector<T> const &v) {
    os << "{ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
    return os << "}";
  }

 public:
  do_print(std::vector<T> const &v)
      : manipulator<std::vector<T> const &>(call, v) {}
};

template <typename T>
std::ostream &operator<<(std::ostream &os, manipulator<T> const &m) {
  if (!os.good()) return os;
  m.do_op(os);
  return os;
}

template <class T>
do_print<T> print(std::vector<T> const &v) {
  return do_print<T>(v);
}

template <class T>
do_print<T> print(T const &v) {
  return do_print<T>(v);
}
