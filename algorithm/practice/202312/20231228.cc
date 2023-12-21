#include <bits/stdc++.h>
#include <type_traits>

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
        typename = std::enable_if_t<std::is_same_v<T, int>>>
void foo(const T &t) {
    // your implementation here...
    std::cout << t << std::endl;
}

template<typename T>
void foo2(const T &t) {
    // your implementation here...
    std::cout << t << std::endl;
}

template<typename T, typename std::enable_if<std::is_const_v<T>, bool>::type = true>
void foo3(const T) {
    std::cout << "T is const" << std::endl;
}

template<typename T, typename std::enable_if<std::is_same_v<T, int>, bool>::type = true>
void foo4(T) {
    std::cout << "T is const" << std::endl;
}

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
    foo(x);  // this will compile and run
    foo(1);
    foo4(x);
    // foo4("");

}

