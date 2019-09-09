# ostr

Header only library for type dependent output

## Example

```C++
  std::vector<int> v{1, 2, 3, 4};
  std::vector<char> vc{'a', 'b', 'c'};

  std::vector<bool> vb{true, false, true};
  std::string s = "hello";

  const char * foo = "foo";
  std::cout << print(s, 42, foo, "foo",true, 'c', 2.0, v, vc, vb) << '\n';
```

Output:

```
"hello", 42, "foo", "foo", true, 'c', 2, [1, 2, 3, 4], ['a', 'b', 'c'], [true, false, true]
```
