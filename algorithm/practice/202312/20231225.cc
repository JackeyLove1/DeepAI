#include <bits/stdc++.h>
#include <cxxabi.h>
#include <functional>
#include <type_traits>
#include <utility>

template <typename... Types>
class Tuple {
 public:
  static constexpr std::size_t length = sizeof...(Types);
};

void DoNothing() { std::cout << "DoNothing(void)" << std::endl; }

template <typename T>
void DoNothing(T&& t) {
  std::cout << "DoNothing(T&& t)" << std::endl;
}
/*
// C++ 17 if constexpr

template <typename Head, typename... Reminders>
void f(Head&& head, Reminders&&... reminders) {
  DoNothing(std::forward<Head>(head));
  if constexpr (sizeof...(Reminders) > 0) {
    f(std::forward<Reminders>(reminders)...);
  }
}
*/

template <typename T>
void f(T&& t) {
  DoNothing(std::forward<T>(t));
}

template <typename Head, typename... Reminders>
void f(Head&& head, Reminders&&... reminders) {
  DoNothing(std::forward<Head>(head));
  f(std::forward<Reminders>(reminders)...);
}

void TestF() { f(1, 2.0, "3"); }

template <typename T>
struct AccumulationTraits;

template <>
struct AccumulationTraits<int> {
  using AccT = int;
  static AccT constexpr zero = 0;
};

template <>
struct AccumulationTraits<double> {
  using AccT = double;
  static AccT constexpr zero = 0.0;
};

template <>
struct AccumulationTraits<float> {
  using AccT = float;
  static AccT constexpr zero = 0.0f;
};

template <>
struct AccumulationTraits<std::string> {
  using AccT = std::string;
  static AccT const zero;
};

struct SumPolicy {
  template <typename T1, typename T2>
  static void accumulate(T1& total, const T2& value) {
    total += value;
  }
};

template <typename T, typename Policy = SumPolicy,
          typename Traits = AccumulationTraits<T>>
decltype(auto) accumulate(T const* begin, T const* end) {
  using AccT = typename Traits::AccT;
  AccT total = Traits::zero;
  while (begin != end) {
    Policy::accumulate(total, *begin);
    ++begin;
  }
  return total;
}

void TestAccumulate() {
  int a[] = {1, 2, 3, 4, 5};
  std::cout << accumulate(a, a + 5) << std::endl;
}

template <typename F, typename... Args,
          typename = decltype(std::declval<F>()(std::declval<Args&&>()...))>
std::true_type isValidImpl(void*);

template <typename F, typename... Args>
std::false_type isValidImpl(...);

template <typename T1, typename T2>
constexpr bool IsSame = std::is_same<T1, T2>::value;

template <typename FROM, typename TO>
constexpr bool IsConvertible = std::is_convertible<FROM, TO>::value;

void TestIsSame() {
  std::cout << IsSame<int, int> << std::endl;
  std::cout << IsSame<int, double> << std::endl;
  std::cout << IsSame<std::string, float> << std::endl;

  std::cout << IsConvertible<int, double> << std::endl;
  std::cout << IsConvertible<std::string, int> << std::endl;
}

template <typename T, std::size_t N>
struct DotProduct {
  static inline T result(T* a, T* b) {
    return *a * *b + DotProduct<T, N - 1>::result(a + 1, b + 1);
  }
};

template <typename T>
struct DotProduct<T, 1> {
  static inline T result(T* a, T* b) { return *a * *b; }
};

template <typename T>
struct DotProduct<T, 0> {
  static inline T result(T*, T*) { return T{}; }
};

template <typename T, std::size_t N>
decltype(auto) dotArrayProduct(T (&a)[N], T (&b)[N]) {
  return DotProduct<T, N>::result(a, b);
}

template <typename T, std::size_t N>
auto dotProduct(std::array<T, N> const& x, std::array<T, N> const& y) {
  return *std::inner_product(x.begin(), x.end(), y.begin(), y.end());
}

void TestDoProduct() {
  std::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::array<int, 5> brr = {2, 3, 4, 5, 6};
  int a[] = {1, 2, 3};
  int b[] = {3, 4, 5};
  std::cout << "Sum int[]: " << dotArrayProduct(a, b) << std::endl;
  std::cout << "Sum array: " << dotProduct(arr, brr) << std::endl;
}

template <int N, int LO = 1, int HI = N>
struct Sqrt {
  static constexpr auto mid = (LO + HI + 1) / 2;
  static constexpr auto value = (N < mid * mid) ? Sqrt<N, LO, mid - 1>::value
                                                : Sqrt<N, mid + 1, HI>::value;
};

template <int N, int M>
struct Sqrt<N, M, M> {
  static constexpr auto value = M;
};

template <long N>
struct Pow3 {
  static long constexpr value = 3 * Pow3<N - 1>::value;
};

template <>
struct Pow3<0> {
  static long constexpr value = 1;
};

void TestSqrt() {
  // std::cout << Sqrt<100>::value << std::endl;
  std::cout << "3^10: " << Pow3<10>::value << std::endl;
  std::cout << "3^20: " << Pow3<20>::value << std::endl;
}

template <typename T, T v>
struct integral_constant {
  static constexpr auto value = v;
  typedef T value_type;
  typedef integral_constant type;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <size_t N>
struct factorial {
  static constexpr auto value =
      static_cast<uint64_t>(N) * factorial<N - 1>::value;
};

template <>
struct factorial<0> {
  static constexpr auto value = 1l;
};

void TestFactorial() {
  std::cout << "5! = " << factorial<5>::value << std::endl;
}

template <typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type Say(T) {
  std::cout << "Call pointer" << std::endl;
}

template <typename T>
typename std::enable_if<!std::is_pointer<T>::value, T>::type Say(T const& t) {
  std::cout << "Call non-pointer, const value: " << t << std::endl;
  return t;
}
template <typename T>
typename std::enable_if<!std::is_pointer<T>::value, void>::type Say(T& t) {
  std::cout << "Call non-pointer, value: " << t << std::endl;
}

void TestSay() {
  int num = 0;
  Say(num);
  Say(std::cref(num));
  Say(&num);
}

template <typename... T>
struct commom_type;

template <typename T, typename U>
struct commom_type<T, U> {
 private:
  static T&& t();
  static U&& u();
  static bool f();

 public:
  typedef typename std::decay<decltype(f() ? t() : u())>::type type;
};

template <typename T, typename U, typename... V>
struct commom_type<T, U, V...> {
  typedef
      typename commom_type<typename commom_type<T, U>::type, V...>::type type;
};

void TestDeclval() {
  std::cout << "int: " << typeid(decltype(std::declval<int>())).name()
            << std::endl;

  std::cout << "double: "
            << typeid(decltype(true ? std::declval<double>()
                                    : std::declval<int>()))
                   .name()
            << std::endl;

  std::cout << "double: "
            << typeid(decltype(false ? std::declval<double>()
                                     : std::declval<int>()))
                   .name()
            << std::endl;
}

template <typename T>
std::string type_name() {
  int status;
  char* name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
  return std::string{name};
}

void TestCommonType() {
  std::cout << "double: " << type_name<commom_type<int, double>::type()>()
            << std::endl;
  std::cout << "double: "
            << type_name<commom_type<int, double, float>::type()>()
            << std::endl;
}

template <typename T>
struct is_int {
  static constexpr bool value = false;
};

template <>
struct is_int<int> {
  static constexpr bool value = true;
};

void TestIsInt() {
  std::cout << "int: " << is_int<int>::value << std::endl;
  std::cout << "double: " << is_int<double>::value << std::endl;
}

template <typename T, typename = std::string>
struct has_to_string : std::false_type {};

template <typename T>
struct has_to_string<T, decltype(std::declval<T>().to_string())>
    : std::true_type {};

template <typename T, typename = std::void_t<>>
struct has_void_to_string : std::false_type {};

template <typename T>
// struct has_void_to_string<T, decltype(std::declval<T>().to_string())> :
// std::true_type {};
struct has_void_to_string<
    T, typename std::enable_if<
           std::is_same<decltype(std::declval<T>().to_string()), void>::value,
           void>::type> : std::true_type {};

void TestHasVoidToString() {
  struct T1 {
    void to_string() {}
  };
  struct T2 {
    int to_string() { return 0; }
  };
  struct T3 {};

  std::cout << has_void_to_string<T1>::value
            << " T1 should have void to_string()" << std::endl;
  std::cout << has_void_to_string<T2>::value
            << " T2 should not have void to_string()" << std::endl;
  std::cout << has_void_to_string<T3>::value
            << " T3 should not have void to_string()" << std::endl;
}

void TestHasToString() {
  struct A {};
  struct B {
    std::string to_string() { return "Hello, World!"; }
  };
  struct C {
    void to_string() {}
  };
  std::cout << "A has to_string: " << has_to_string<A>::value << std::endl;
  std::cout << "B has to_string: " << has_to_string<B>::value << std::endl;
  std::cout << "C has to_string: " << has_to_string<C>::value << std::endl;

  std::cout << "A has void to_string: " << has_void_to_string<A>::value
            << std::endl;
  std::cout << "B has void to_string: " << has_void_to_string<B>::value
            << std::endl;
  std::cout << "C has void to_string: " << has_void_to_string<C>::value
            << std::endl;
}

int main() {
  TestF();

  TestAccumulate();

  TestIsSame();

  TestDoProduct();

  TestSqrt();

  TestFactorial();

  TestSay();

  TestDeclval();

  TestCommonType();

  TestIsInt();

  TestHasToString();

  TestHasVoidToString();

  return 0;
}