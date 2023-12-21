#include <bits/stdc++.h>
#include <type_traits>
#include <functional>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <any>
#include <variant>
#include <utility>
#include <span>

template<typename T, class U = typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
struct AccumulationTraits {
    using type = unsigned long long;

    static constexpr auto zero() noexcept { return type{0}; }

    static constexpr auto one() noexcept { return type{1}; }
};


/*
template<typename T>
struct AccumulationTraits<T,
        typename std::enable_if_t<std::is_integral_v<T>
                                  && std::is_signed_v<T>, T>> {
    using type = long long;

    static constexpr auto zero() noexcept { return type{0}; }

    static constexpr auto one() noexcept { return type{1}; }
};

template<typename T>
struct AccumulationTraits<T,
        typename std::enable_if_t<std::is_integral_v<T>
                                  && std::is_floating_point_v<T>, T>> {
    using type = long double;

    static constexpr auto zero() noexcept { return type{0.0}; }

    static constexpr auto one() noexcept { return type{1.0}; }
};


struct SummationPolicy {
    template<typename T>
    inline static constexpr auto init() noexcept { AccumulationTraits<T, T>::zero(); }

    template<typename T, typename U>
    inline static void operation(T &s, U const &v) {
        s += v;
    }
};

struct MultiplicationPolicy {
    template<typename T>
    inline static constexpr auto init() noexcept { AccumulationTraits<T, T>::one(); }

    template<typename T, typename U>
    inline static void operation(T &s, U const &v) {
        s += v;
    }
};

//template<typename T, typename Policy = SummationPolicy, typename AT = typename AccumulationTraits<T>::type>
//auto Accumulation(const std::initializer_list<T> &arr) {
//    auto result = Policy::init<T>();
//    std::for_each(arr.begin(), arr.end(), [&result](const auto &value) {
//        result = Policy::operation(result, value);
//    });
//    return result;
//}
*/
void TestAccumulation() {
//    std::cout << AccumulationTraits<int>::one() << "\n";
//    std::cout << "accum({1, 2, 3, 4, 5}) = " << Accumulation({1, 2, 3, 4, 5}) << '\n';
//    std::cout << "accum({'a', 'b', 'c', 'd', 'e'}) = " << Accumulation({'a', 'b', 'c', 'd', 'e'}) << '\n';
//    std::cout << "accum({1.1, 2.2, 3.3, 4.4, 5.5}) = " << Accumulation({1.1, 2.2, 3.3, 4.4, 5.5}) << '\n';
//    std::cout << "accum<MultiplicationPolicy>({1, 2, 3, 4, 5}) = "
//              << Accumulation<MultiplicationPolicy>({1, 2, 3, 4, 5}) << '\n';
//    std::cout << "accum<MultiplicationPolicy>({'a', 'b', 'c', 'd', 'e'}) = "
//              << Accumulation<MultiplicationPolicy>({'a', 'b', 'c', 'd', 'e'}) << '\n';
//    std::cout << "accum<MultiplicationPolicy>({1.1, 2.2, 3.3, 4.4, 5.5}) = "
//              << Accumulation<MultiplicationPolicy>({1.1, 2.2, 3.3, 4.4, 5.5}) << '\n';
}

void TestSwitch() {
    const int i = 1;
    switch (i) {
        case 1:
        case 2:
            std::cout << "1 or 2";
            break;
        case 3:
        case 4:
            std::cout << "3 or 4";
            break;
        case 5:
            std::cout << "45";
            break;           // execution of subsequent statements is terminated
        case 6:
            std::cout << '6';
    }
}

template<typename R, typename P>
class Memoizer {
    constexpr R f(const P &n) {
        if (n < 2) {
            return n;
        } else {
            return call(n - 2) + call(n - 1);
        }
    }

    std::unordered_map<P, R> m;
public:
    R call(const P &param) {
        if (auto iter = m.find(param); iter != m.end()) {
            return iter->second;
        } else {
            auto r = f(param);
            m.emplace(param, r);
            return r;
        }
    }
};


void TestMem() {
    using ll = long long;
    Memoizer<ll, ll> m;
    for (ll i = 0; i < 50; ++i) {
        std::cout << "Fib(" << i << "): " << m.call(i) << std::endl;
    }
}

consteval auto f(int num){
    auto n{0};
    for(int i = 0; i != num; ++i){
        n += 1;
    }
    return n + num;
}

void TestF(){
    std::cout << f(10) << "\n";
}


int main() {
    TestAccumulation();

    TestSwitch();

    TestMem();

    TestF();

    return 0;
}