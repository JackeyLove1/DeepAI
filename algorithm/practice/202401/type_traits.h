#include <type_traits>

namespace syncservice {

template <typename... Ts>
struct make_void {
  typedef void type;
};
template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

template <typename T, typename = void>
struct has_iterator : public std::false_type {};

template <typename T>
struct has_iterator<T, void_t<typename T::iterator>> : public std::true_type {};

template <typename T, typename = void>
struct has_const_iterator : public std::false_type {};

template <typename T>
struct has_const_iterator<T, void_t<typename T::const_iterator>> : public std::true_type {};

template <typename T>
class GLogPolicy {
 public:
  static void print(T const& value) { LOG(INFO) << value; }
};

template <typename ContainerType, template <typename...> class Policy = GLogPolicy,
          typename = std::enable_if_t<has_iterator<ContainerType>::value>>
auto LOGContainer(ContainerType const& container) {
  std::for_each(container.begin(), container.end(),
                [](const auto& value) { Policy<decltype(value)>::print(value); });
}

}  // namespace syncservice
