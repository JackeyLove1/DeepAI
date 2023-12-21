#include <bits/stdc++.h>
#include <cxxabi.h>
#include <functional>
#include <glog/logging.h>
#include <utility>

template <typename T>
std::string type_name() {
  int status;
  char* name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
  return std::string{name};
}

template <typename T1, typename T2>
void TestType(T1, T2) {
  if (!std::is_same<T1, T2>::value) {
    LOG(INFO) << "Types are different: " << type_name<T1>() << " and "
              << type_name<T2>();
  } else {
    LOG(INFO) << "Types are the same: " << type_name<T1>();
  }
}

void TestTypeImpl() {
  TestType(1, 1);
  TestType("Hello", 1);
  TestType(1, 1.0);
  TestType(1, 1.0f);
  TestType("Hello", 1.0l);
  TestType("Hello", 1u);
}

template <typename T>
void Print(T t) {
  std::cout << t << " ";
}

template <typename T, typename... Types>
void Print(T firstArgs, Types... args) {
  std::cout << firstArgs << " ";
  Print(args...);
}

template <typename T, typename... Types>
void PrintSizeof(T firstArgs, Types... args) {
  std::cout << firstArgs << " ";
  if (sizeof...(args) > 0) {
    Print(args...);
  }
}

template <typename T>
class AddSpace {
 private:
  T const& ref;

 public:
  explicit AddSpace(T const& r) : ref(r) {}
  friend std::ostream& operator<<(std::ostream& os, AddSpace<T> const& obj) {
    return os << obj.ref << " ";
  }
};

template <typename T>
void PrintWithSpace(T t) {
  std::cout << AddSpace<T>(t) << "\n";
}
template <typename T, typename... Args>
void PrintWithSpace(T firstArgs, Args... args) {
  std::cout << AddSpace<T>(firstArgs);
  PrintWithSpace(args...);
}

template <typename T1, typename T2>
constexpr bool IsSameType(T1, T2) {
  return std::is_same<T1, T2>::value;
}

template <typename T, typename... Args>
constexpr bool IsSameType(T, Args...) {
  return (std::is_same<T, Args>::value && ...);
}

void TestSameType() {
  LOG(INFO) << "Same type: " << IsSameType(1, "2", 3.0);
  LOG(INFO) << "Same type: " << IsSameType(1, 2, 3);
}

template <typename T>
constexpr T pi{3.1415926535897932385};

void PrintPI() {
  LOG(INFO) << "PI: " << pi<double>;
  LOG(INFO) << "PI: " << pi<float>;
}

template <int N>
std::array<int, N> arr{};

// use compiling to calculate prime
template <unsigned p, unsigned d>
struct DoIsPrime {
  static constexpr bool value = (p % d != 0) && DoIsPrime<p, d - 1>::value;
};

template <unsigned p>
struct DoIsPrime<p, 2> {
  static constexpr bool value = (p % 2 != 0);
};

template <unsigned p>
struct DoIsPrime<p, 1> {
  static constexpr bool value = true;
};
template <unsigned p>
struct DoIsPrime<p, 0> {
  static constexpr bool value = true;
};

// special case
template <unsigned p>
struct IsPrime {
  static constexpr bool value = DoIsPrime<p, p / 2>::value;
};

template <>
struct IsPrime<0> {
  static constexpr bool value = false;
};

template <>
struct IsPrime<1> {
  static constexpr bool value = false;
};

template <>
struct IsPrime<2> {
  static constexpr bool value = true;
};

void TestPrime() {
  LOG(INFO) << 9 << " is prime: " << IsPrime<9>::value;
  LOG(INFO) << 100 << " is prime: " << IsPrime<100>::value;
  LOG(INFO) << 1007 << " is prime: " << IsPrime<1007>::value;
  LOG(INFO) << 2 << " is prime: " << IsPrime<2>::value;
}

template <typename T, std::size_t N>
std::size_t len(T (&arg)[N]) {
  return N;
}

template <typename T>
auto len(T& t) {
  return t.size();
}

std::size_t len(...) { return 0; }

void TestLen() {
  int a[10];
  std::array<int, 100> b{};
  std::string c{" ", 100};
  std::allocator<int> d;
  LOG(INFO) << "a len: " << len(a);
  LOG(INFO) << "b len: " << len(b);
  LOG(INFO) << "c len: " << len(c);
  LOG(INFO) << "d len: " << len(std::ref(d));
}

template <typename T>
void PrintIt(T const& t) {
  std::cout << t << "\n";
}

template <typename T, typename... Types>
void PrintIt(T const& firstArg, Types const&... args) {
  std::cout << firstArg << " ";
  PrintIt(args...);
  /*
  if constexpr (sizeof...(args) > 0) {
    PrintIt(args...);
  }
   */
}

void TestPrintIt() {
  PrintIt("Hello, World!");
  PrintIt(1, 2.0, 3u, "4");
}

template <typename T>
void PrintInfo(T&& t) {
  std::cout << "TypeInfo: " << typeid(std::forward<T>(t)).name() << "\n";
}

template <typename T>
void PrintInfo(T const& t) {
  std::cout << "TypeInfo: " << typeid(t).name() << "\n";
}

void TestPrintInfo() {
  PrintInfo("Hello");
  PrintInfo(1.0);
  PrintInfo(1u);
}

template <typename Callable, typename... Args>
decltype(auto) call(Callable&& op, Args&&... args) {
  return std::invoke(std::forward<Callable>(op), std::forward<Args>(args)...);
}

void TestCall() {
  call([](std::string const& name) { std::cout << "Hello, " << name << "\n"; },
       "World!");
  call(
      [](int a, int b) {
        std::cout << "Sum is: " << a + b << "\n";
        return a + b;
      },
      1, 2);
}
template <typename T>
typename std::conditional<std::is_move_constructible<T>::value, T&&, T&>::type forward(T& t) {
  return std::is_move_constructible<T>::value? std::move(t) : t;
}

int main() {
  TestTypeImpl();
  Print(1, 2, 3);
  Print("1", 2, 3.0, 4u, 5l);
  PrintSizeof(1.0, "2", 3u);
  PrintWithSpace("1");
  PrintWithSpace("1", 2, 3.0, 4u, 5l);

  TestSameType();

  PrintPI();

  TestPrime();

  TestLen();

  TestPrintIt();

  TestPrintInfo();

  TestCall();

  return 0;
}