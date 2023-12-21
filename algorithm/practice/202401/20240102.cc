#include <bits/stdc++.h>
#include <type_traits>
#include <functional>
#include <memory>

using std::equal;
using std::rotate;

template<typename InputIterator, typename EqualityComparable>
bool linear_search(InputIterator first, InputIterator last, const EqualityComparable &value) {
    return std::find(first, last, value) != last;
}

template<typename T, T v>
struct integral_constant {
    static const T value = v;
    using value_type = T;
    using type = integral_constant<T, v>;
};

template<typename T, T v> const T integral_constant<T, v>::value;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<typename T>
struct is_integral : public false_type {
};
template<>
struct is_integral<int> : public true_type {
};
template<>
struct is_integral<bool> : public true_type {
};
template<>
struct is_integral<short> : public true_type {
};
template<>
struct is_integral<long> : public true_type {
};

template<typename T>
struct is_pointer : public false_type {
};
template<typename T>
struct is_pointer<T *> : public true_type {
};

template<typename T>
struct is_const : public false_type {
};
template<typename T>
struct is_const<const T> : public true_type {
};

template<typename T>
struct is_const_reference : std::false_type {
};
template<typename T>
struct is_const_reference<const T &> : std::true_type {
};

template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T, typename std::enable_if<is_const_reference<T>::value, bool>::type = true>
void TestConstParameter(const T &) {
    std::cout << "only const T& " << std::endl;
}

template<typename T,
        typename std::enable_if<std::is_same<T, int>::value, int>::type = 0>
void foo(const T &t) {
    std::cout << t << std::endl;
}

template<typename T>
void foo2(const T &t) {
    // your implementation here...
    std::cout << t << std::endl;
}


template<typename Dest, typename Source>
struct is_bitcastable
        : public std::integral_constant<
                bool, sizeof(Dest) == sizeof(Source) &&
                      std::is_trivially_constructible<Dest>::value &&
                      std::is_trivially_constructible<Source>::value &&
                      std::is_default_constructible<Dest>::value> {
};

template<
        typename Dest, typename Source,
        typename std::enable_if<is_bitcastable<Dest, Source>::value, bool>::type = true>
inline Dest bit_cast(const Source &src) {
    Dest dest;
    std::memcpy(static_cast<void *>(std::addressof(dest)), static_cast<const void *>(std::addressof(src)),
                sizeof(dest));
    return dest;
}

template<int N>
struct marshall {
    char buf[N];
};

template<typename T>
void TestMarshall(const T values[]) {
    const size_t sz = sizeof(values) / sizeof(T);
    std::cout << "size: " << sz << "\n";
    for (size_t i = 0; i < sz; ++i) {
        auto t0 = values[i];
        auto m0 = bit_cast<marshall<sizeof(T)> >(t0);
        auto t1 = bit_cast<T>(m0);
        auto m1 = bit_cast<marshall<sizeof(T)> >(t1);
        std::cout << "t0 vs m0: " << std::memcmp(std::addressof(t0), std::addressof(m0), sizeof(T)) << " Expectd:0\n";
        std::cout << "t1 vs m1: " << std::memcmp(std::addressof(t1), std::addressof(m1), sizeof(T)) << " Expectd:0\n";
    }
}

template<typename T, T... Ints>
struct integer_sequence {
    using value_type = T;

    static constexpr size_t size() noexcept { return sizeof...(Ints); }
};


// debugging aid
template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq) {
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}

// convert array into a tuple
template<typename Array, std::size_t... I>
auto a2t_impl(const Array &a, std::index_sequence<I...>) {
    return std::make_tuple(a[I]...);
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N> &a) {
    return a2t_impl(a, Indices{});
}

// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr> &os,
                      const Tuple &t,
                      std::index_sequence<Is...>) {
    ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}

template<class Ch, class Tr, class... Args>
auto &operator<<(std::basic_ostream<Ch, Tr> &os,
                 const std::tuple<Args...> &t) {
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

template<typename... Ts>
std::ostream &operator<<(std::ostream &os, std::tuple<Ts...> const &Tuples) {
    std::apply([&os](Ts const &... tupleArgs) {
        os << "[";
        std::size_t n(0);
        ((os << tupleArgs << ((++n == sizeof...(Ts)) ? "]" : " ")), ...);
        os << "\n";
    }, Tuples);
    return os;
}

template<typename T>
T add(T a, T b) { return a + b; }

template<>
auto add(auto a, auto b) { return a + b; }

void TestSTD() {
    print_sequence(std::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
    print_sequence(std::make_integer_sequence<int, 20>{});
    print_sequence(std::make_index_sequence<10>{});
    print_sequence(std::index_sequence_for<float, std::iostream, char>{});

    std::array<int, 4> array = {1, 2, 3, 4};

    // convert an array into a tuple
    auto tuple = a2t(array);
    static_assert(std::is_same_v<decltype(tuple),
            std::tuple<int, int, int, int>>, "");

    // print it to cout
    std::cout << "The tuple: " << tuple;

    std::cout << std::make_tuple(1, 2, 3, 4);
}

template<typename T>
class AddSpace {
private:
    T const &ref;
public:
    explicit AddSpace(T const &r) : ref(r) {};

    friend std::ostream &operator<<(std::ostream &os, AddSpace<T> s) {
        return os << s.ref << " ";
    }
};

template<typename ... Args>
void print_values(Args const &... args) {
    (std::cout  << ... << AddSpace(args)) << "\n";
}

void TestPrint() {
    print_values("Hello", 1, 2.3f, 4u);
}

template<typename ... Ts>
void PrintDouble(Ts ... ts) {
    print_values((ts + ts) ...);
}

void TestPrintDouble() {
    PrintDouble(1, 2, 3, 4);
}

template<typename T1, typename ... Ts>
constexpr bool isHomogeneous(T1, Ts ...) {
    return (std::is_same_v<T1, Ts> && ... );
}

void TestIsHomogeneous() {
    std::cout << isHomogeneous(1, 2.0f) << "\n";
    std::cout << isHomogeneous(1, "Hello") << "\n";
    std::cout << isHomogeneous(1, 2, 3, 4) << "\n";
}

// for print tuple by index
template<std::size_t ...>
struct Indices {
};

template<typename T, std::size_t ... Idx>
void PrintTuple(T const &t, Indices<Idx ...>) {
    print_values(std::get<Idx>(t)...);
}

template<typename T, std::size_t ... Idx>
void PrintByIndex(T const &t, std::size_t ...) {
    print_values(std::get<Idx>(t)...);
}

void TestPrintTuple() {
    auto t1 = std::make_tuple(1, 2.3f, "4");
    PrintTuple(t1, Indices<0, 1, 2>());

}

template<typename T>
using EnableIfString = std::enable_if_t<std::is_convertible_v<T, int>>;

class Value {
private:
    int value_;
public:
    template<typename T, typename = EnableIfString<T>>
    explicit Value(T &&value):value_(std::forward<T>(value)) {};

    int get_value() const {
        return value_;
    }
};

void TestValue() {
    Value v1(1);
    print_values(v1.get_value());
    Value v2(2.3);
    print_values(v2.get_value());
    Value v3(4u);
    print_values(v3.get_value());
}

template<typename T, std::size_t L1, std::size_t L2>
void CompareArrays(T (&arg1)[L1], T (&arg2)[L2]) {
    T *pa = arg1;
    T *pb = arg2;
}

template<typename T>
std::decay_t<T> returnType(T) {
    return T();
}

template<typename T1, typename T2>
std::pair<std::decay_t<T1>, std::decay_t<T2>> make_pair(T1 &&t1, T2 &&t2) {
    return std::pair<typename std::decay<T1>::type,
            typename std::decay<T2>::type>
            (std::forward<T1>(t1), std::forward<T2>(t2));
}

template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, std::pair<T1, T2> const &pair) {
    return os << "PrintPair: " << pair.first << " " << pair.second;
}

void TestMakeTuple() {
    auto p1 = make_pair(1u, 2.0f);
    std::cout << p1 << "\n";
    std::cout << make_pair("Hello", 1) << "\n";

}

template<typename T, typename ... Ts>
void PrintArgs(T const &firstArg, Ts const &... secondArgs) {
    std::cout << firstArg << " ";
    if constexpr (sizeof...(secondArgs) > 0) {
        PrintArgs(secondArgs ...);
    } else {
        std::cout << "\n";
    }
}

void TestPrintArgs() {
    PrintArgs(1, 2.3, '4', "56");
}

template<typename Callable, typename ... Args>
decltype(auto) Call(Callable &&op, Args &&... args) {
    if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
        std::invoke(std::forward<Callable>(op), std::forward<Args>(args) ...);
        return;
    } else {
        return std::invoke(std::forward<Callable>(op), std::forward<Args>(args) ...);
    }
}

void TestCall() {
    Call([]() { std::cout << "Hello, Callable\n"; });
    auto res = Call([](auto a, auto b) { return a + b; }, 1, 2);
    std::cout << res << std::endl;
}

void TestSteal() {
    std::string s1 = "Hello, World";
    auto &&s2 = s1;
    auto &&s3 = std::move(s1);
}

template<typename T1, typename T2, template<typename ...> class Cont>
struct Ref1 {
};

bool and_all() { return true; }

template<typename T>
bool and_all(T cond) { return cond; }

template<typename T, typename ... Ts>
bool and_all(T cond, Ts ... conds) {
    return cond && and_all(conds ...);
}

template<typename T> constexpr std::size_t SZ = sizeof(T);
template<> constexpr std::size_t SZ<void> = 0;

int main() {
    int a[] = {0, 1, 2, 3, 4};
    std::cout << static_cast<void *>(a) << "\n";
    std::cout << static_cast<void *>(a + 1) << "\n";
    std::cout << (*(a + 1)) << "\n";
    std::string s1 = "Hello";
    const std::string &s2 = s1;
    const std::string s3 = "Hello";
    // TestConstParameter(std::cref(s2));
    const int x = 10;

    const int nums1[] = {1};
    TestMarshall(nums1);

    TestSTD();

    TestPrint();

    TestPrintDouble();

    TestIsHomogeneous();

    TestPrintTuple();

    TestValue();

    TestMakeTuple();

    TestPrintArgs();

    TestCall();

    TestSteal();

    return 0;
}

