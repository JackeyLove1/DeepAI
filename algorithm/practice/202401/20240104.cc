#include <bits/stdc++.h>
#include <type_traits>
#include <functional>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <any>
#include <variant>
#include <utility>

template<typename T>
struct AccumulationTraits;

template<>
struct AccumulationTraits<char> {
    using AccT = int;

    static constexpr AccT zero() { return 0; };
};

template<>
struct AccumulationTraits<short> {
    using AccT = int;

    static constexpr AccT zero() { return 0; };
};

template<>
struct AccumulationTraits<int> {
    using AccT = long;

    static constexpr AccT zero() { return 0; };
};

template<>
struct AccumulationTraits<float> {
    using AccT = double;

    static constexpr AccT zero() { return 0.0; };
};

template<typename T>
auto accm(T const *beg, T const *end) {
    using AccT = typename AccumulationTraits<T>::AccT;
    AccT total = AccumulationTraits<T>::zero;
    while (beg != end) {
        total += *beg;
        ++beg;
    }
    return total;
}

template<typename T, typename AT = AccumulationTraits<T>>
auto accm2(T const *beg, T const *end) {
    typename AT::AccT total = AT::zero();
    while (beg != end) {
        total += *beg;
        ++beg;
    }
    return total;
}

void TestAccm() {

}

void TestType() {
    std::cout << "int:" << std::type_index(typeid(int)).name() << "\n";
    std::cout << "std::string:" << std::type_index(typeid(std::string)).name() << "\n";
    struct A {
    };
    std::cout << "std::string:" << std::type_index(typeid(A)).name() << "\n";
    std::cout << "std::string:" << std::type_index(typeid(A)).name() << "\n";
}

class Any {
public:
    Any(void) : m_type(std::type_index(typeid(void))) {}

    Any(const Any &other) : m_ptr(other.Clone()), m_type(other.m_type) {}

    Any(Any &&other) : m_ptr(std::move(other.m_ptr)), m_type(other.m_type) {};

    template<typename U,
            class T = std::decay_t<U>,
            class = typename std::enable_if<
                    !std::is_same<T, Any>::value &&
                    std::is_copy_constructible_v<T>, U>::type>
    Any(U &&value): m_ptr(new Derived<typename std::decay_t<U>>(std::forward<U>(value))),
                    m_type(std::type_index(typeid(U))) {}

    template<typename ValueType, typename ... Args,
            class U = std::decay_t<ValueType>,
            class = typename std::enable_if_t<
                    std::is_constructible_v<U, Args...>
                    && std::is_copy_constructible_v<U>>>
    Any(ValueType &&value, Args &&... args):
            m_ptr(std::make_unique<Derived < U>>

    (
    std::forward<ValueType>(value), std::forward<Args>(args)
    ...)),

    m_type (std::type_index(

    typeid(ValueType))){}

    inline bool IsNull() const {
        return m_ptr == nullptr;
    }

    template<typename U>
    bool Is() {
        return m_type == std::type_index(typeid(U));
    }

    template<typename U>
    U &AnyCast() {
        if (!Is<U>()) {
            std::cout << "can not cast " << typeid(U).name() << " to " << m_type.name() << std::endl;
            throw std::bad_cast();
        }
        auto *derived = dynamic_cast<Derived <U> *>(m_ptr.get());
        return derived->m_value;
    }

    Any &operator=(const Any &other) {
        if (m_ptr == other.m_ptr) return *this;
        m_ptr = other.Clone();
        m_type = other.m_type;
        return *this;
    }

private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;

    struct Base {
        virtual ~Base() {}

        virtual BasePtr Clone() const = 0;
    };

    template<typename T>
    struct Derived : public Base {
        template<typename U>
        Derived(U &&value) : m_value(std::forward<U>(value)) {};

        BasePtr Clone() const { return BasePtr(new Derived<T>(m_value)); }

        T m_value;
    };

public:
    BasePtr Clone() const {
        if (m_ptr != nullptr) return m_ptr->Clone();
        return nullptr;
    }

    BasePtr m_ptr;
    std::type_index m_type;
};

void TestAny() {
    Any a1 = 1;
    auto v1 = a1.AnyCast<int>();
    std::cout << "v1: " << v1 << "\n";

    std::string s1 = "Hello";
    Any a2 = s1;
    auto s2 = a2.AnyCast<std::string>();
    std::cout << "s1: " << s2 << "\n";

    std::any a3 = std::string("Hello");
    std::cout << std::any_cast<std::string>(a3) << "\n";
    Any a4 = std::string("Hello");
    std::cout << "a4: " << a4.AnyCast<std::string>() << "\n";
    class A {
    public:
        int a_;

        explicit A(int a) : a_(a) {}

        std::string ToString() const {
            return "A: " + std::to_string(a_);
        }
    };

    Any a5 = A(1);
    std::cout << a5.AnyCast<A>().ToString() << "\n";
}

void TestVariant() {
    std::variant<int, float> v, w;
    v = 42;
    auto i = std::get<int>(v);
    std::cout << "i: " << i << std::endl;
    std::variant<std::string, int> y("abc");
    y = 1;
    std::cout << "y: " << std::get<int>(y) << "\n";
}


// Our fallback method, this will always exist:
struct Fallback {
    void ToString();
};

// The type we use to cast to in our test:
struct Derived : Fallback {
};

template<typename T, T>
struct Check;

typedef char Yes[1];
typedef char No[2];

template<typename C>
static Yes &Test(Check<void (C::*)(), &C::ToString> *);

template<typename>
static No &Test(...);

template<typename C>
struct HasToString {
    enum {
        value = sizeof(Test<Derived>(0)) == sizeof(Yes)
    };
};

class TestClass {
public:
    std::string ToString() {
        return "Hello, world!";
    }
};

template<typename T>
class Circle {
    T m_radius;

    static constexpr auto make_pi() {
        if constexpr (std::is_same_v<T, float>) {
            return asinf(1.0f) * 2.0f;
        } else if constexpr (std::is_same_v<T, double>) {
            return asinl(1.0L) * 2.0L;
        } else {
            return asin(T{1}) * T{2};
        }
    }

public:
    inline static const auto PI = make_pi();

    explicit Circle(T &&radius) : m_radius{std::move(radius)} {}

    const auto &radius() const noexcept {
        return m_radius;
    }

    const auto circumference() const noexcept {
        return T{2} * PI * m_radius;
    }

    const auto area() const noexcept {
        return PI * m_radius * m_radius;
    }
};

void TestCircle() {
    Circle c1(2.0);
    Circle c2(10);
    Circle c3(1.0f);
    Circle c4(100.0L);
}

template<typename T>
typename std::enable_if_t<std::is_integral_v<T>, T>
index(T x, unsigned p) {
    if (p == 0) {
        std::cerr << "index<T>()\n";
        return 1;
    } else {
        return x * index(x, p - 1);
    }
}

template<typename T, class = typename std::enable_if_t<!std::is_integral_v<T> && std::is_arithmetic_v<T>>>
decltype(auto) index(T x, unsigned p) {
    return std::pow(x, p);
}

/*
template<typename T>
typename std::enable_if_t<!std::is_integral_v<T> && !std::is_void_v<decltype(std::pow(std::declval<T>(), 0u))>, T>
index(T x, unsigned p) {
    return pow(x, p);
}
*/


void TestPow() {
    std::cout << "pow(-3,4): " << index(-3, 4) << std::endl;
    std::cout << "pow(2,5,4): " << index(2.5, 4) << std::endl;
}

template<typename T, typename U>
struct AccumulateTypeTraits {
    using type = U;
};



int main() {
    TestType();

    TestAny();

    TestVariant();

    TestCircle();

    TestPow();

}