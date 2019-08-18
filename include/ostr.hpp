#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class manipulator {
  std::function<std::ostream&(std::ostream&, T)> m_func;
  T m_arg;

 public:
  manipulator(std::function<std::ostream&(std::ostream&, T)> f, T arg)
      : m_func(f), m_arg(arg) {}
  void do_op(std::ostream& str) const { m_func(str, m_arg); }
};

template <typename T>
class do_print : public manipulator<const std::vector<T>&> {
  static std::ostream& call(std::ostream& os, const std::vector<T>& v) {
    os << "{ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
    return os << "}";
  }
 public:
  do_print(const std::vector<T>& v)
      : manipulator<const std::vector<T>&>(call, v) {}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const manipulator<T>& m) {
  if (!os.good()) return os;
  m.do_op(os);
  return os;
}

template <class T>
do_print<T> print(const std::vector<T>& v) {
  return do_print<T>(v);
}
