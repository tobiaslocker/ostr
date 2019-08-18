#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <class Argument>
class manipulator {
  typedef std::ostream& (*Function)(std::ostream&, Argument);

 public:
  manipulator(std::function<std::ostream&(std::ostream&, Argument)> f,
              Argument _arg)
      : func(f), arg(_arg) {}

  void do_op(std::ostream& str) const { func(str, arg); }

 private:
  std::function<std::ostream&(std::ostream&, Argument)> func;
  Argument arg;
};

template <class T>
class do_print : public manipulator<const std::vector<T>&> {
 public:
  do_print(const std::vector<T>& v)
      : manipulator<const std::vector<T>&>(call, v) {}

 private:
  static std::ostream& call(std::ostream& os, const std::vector<T>& v) {
    os << "{ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
    return os << "}";
  }
};

template <class Argument>
std::ostream& operator<<(std::ostream& os, const manipulator<Argument>& m) {
  if (!os.good()) return os;

  m.do_op(os);
  return os;
}

template <class T>
do_print<T> Print(const std::vector<T>& v) {
  return do_print<T>(v);
}
