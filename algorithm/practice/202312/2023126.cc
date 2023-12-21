#include <bits/stdc++.h>
#include <ev.h>

template <typename... Types>
struct SimpleTuple;

template <typename T>
struct SimpleTuple<T> {
  T head;
};

template <typename Head, typename... Tails>
struct SimpleTuple<Head, Tails...> {
  Head head;
  SimpleTuple<Tails...> tail;
};

template <>
struct SimpleTuple<> {};

template <typename... Types>
SimpleTuple<Types...> make_SimpleTuple(Types... args) {
  return SimpleTuple<Types...>{args...};
}

template <size_t index, typename... Types>
struct Get;

template <typename Head, typename... Tail>
struct Get<0, Head, Tail...> {
  static Head value(SimpleTuple<Head, Tail...>& t) { return t.head; }
};

template <size_t index, typename Head, typename... Tail>
struct Get<index, Head, Tail...> {
  static auto value(SimpleTuple<Head, Tail...>& t) {
    return Get<index - 1, Tail...>::value(t.tail);
  }
};

template <unsigned N>
struct TupleGet;

template <unsigned N>
struct TupleGet {
  template <typename Head, typename... Tails>
  static auto apply(SimpleTuple<Head, Tails...>& t) {
    return TupleGet<N - 1>::apply(t.tail);
  }
};

template <>
struct TupleGet<0> {
  template <typename... Types>
  static auto apply(SimpleTuple<Types...>& t) {
    return t.head;
  }
};

template <unsigned N, typename... Types>
decltype(auto) get(SimpleTuple<Types...>& t) {
  return TupleGet<N>::apply(t);
}

void TestSimpleTuple() {
  SimpleTuple<int, float, char> t = make_SimpleTuple(1, 2.0f, 'a');
  std::cout << "1st: " << Get<0, int, float, char>::value(t) << "\n";
  std::cout << "2nd: " << Get<1, int, float, char>::value(t) << "\n";
  std::cout << "3rd: " << Get<2, int, float, char>::value(t) << "\n";
}

void TestSimpleTupleGet() {
  SimpleTuple<int, float, char> t = make_SimpleTuple(1, 2.0f, 'a');
  std::cout << "1st: " << get<0>(t) << "\n";
  std::cout << "2nd: " << get<1>(t) << "\n";
  std::cout << "3rd: " << get<2>(t) << "\n";
}
/*
template <typename... Types>
class Tuple;

template <typename T>
class Tuple<T> {
 private:
  T t;

 public:
  Tuple() = default;
  Tuple(T&& t_) : t(std::forward<T>(t_)) {}
  T getHead() { return t; }
  T getHead() const { return t; }
  T getTail() { return t; }
  T getTail() const { return t; }
};

template <typename Head, typename... Tails>
class Tuple<Head, Tails...> {
 private:
  Head head;
  Tuple<Tails...> tails;

 public:
  Tuple() = default;
  Tuple(Head&& head_, Tuple<Tails...>&& tails_...)
      : head(std::forward<Head>(head_)),
        tails(std::forward<Tails>(tails_)...) {}
  Head getHead() { return head; }
  Head getHead() const { return head; }
  Tuple<Tails...> getTail() { return tails; }
  Tuple<Tails...> getTail() const { return tails; }
};

template <>
class Tuple<> {};

template <unsigned N>
struct TupleGet {
  template <typename Head, typename... Tails>
  static decltype(auto) apply(Tuple<Head, Tails...> const& t) {
    return TupleGet<N - 1>::apply(t.getTail());
  }
};

template <>
struct TupleGet<0> {
  template <typename... Types>
  static decltype(auto) apply(Tuple<Types...> const& t) {
    return t.getHead();
  }
};

template <unsigned N, typename... Types>
decltype(auto) get(Tuple<Types...> const& t) {
  return TupleGet<N>::apply(t);
}

template <typename... Types>
Tuple<Types...> make_Tuple(Types&&... types) {
  return Tuple<Types...>{std::forward<Types>(types)...};
};

template <typename Head, typename ... Tails>
Tuple<Head, Tails...> make_Tuple(Head&& head, Tails&&... tails) {
  return Tuple<Head, Tails...>{std::forward<Head>(head),
std::forward<Tails>(tails)...};
};


void TestTuple() {
  Tuple<int, float, char> t = make_Tuple(1, 2.0f, 'a');
  std::cout << "1st: " << get<0>(t) << "\n";
  std::cout << "2nd: " << get<1>(t) << "\n";
  std::cout << "3rd: " << get<2>(t) << "\n";
}
*/

struct Timer{
  struct ev_loop* ev_loop_;
  ev_timer * ev_timer_;
  Timer(){
    ev_loop_ = ev_loop_new();
    ev_timer_ = (ev_timer*)malloc(sizeof(ev_timer));
  }
  static void TimerCallback(EV_P_ ev_timer *w, int revents) {
    std::cout << "Timer triggered!" << std::endl;
  }

  void StartTimer() {
    ev_timer_init(ev_timer_, TimerCallback, 1.0, 0.0);
    ev_timer_start(ev_loop_, ev_timer_);
    ev_run(ev_loop_, 0);
  }
};

void TestTimer(){
  Timer timer;
  timer.StartTimer();
}

void SimpleLoop(){
  struct ev_loop *loop = EV_DEFAULT;
  ev_run (loop, 0);
}

static void TimerCallback1(EV_P_ ev_timer *w, int revents) {
  std::cout << "Timer triggered!" << std::endl;
}

void TestTimeoutLoop(){
  struct ev_loop *loop = EV_DEFAULT;
  ev_timer timer;
  ev_timer_init(&timer, TimerCallback1, 1.0, 0.0);
  ev_timer_start(loop, &timer);
  ev_run(loop, 0);
}

static void TimerCallback2(EV_P_ ev_timer *w, int revents) {
  std::cout << "Epoll Timer triggered!" << std::endl;

}
void TestEvEpoll(){
  struct ev_loop *loop = ev_loop_new(EVBACKEND_EPOLL);
  ev_timer timer;
  ev_timer_init(&timer, TimerCallback2, 1.0, 0.0);
  ev_timer_start(loop, &timer);
  ev_run(loop, 0);
  ev_loop_destroy(loop);
}

int main() {

  TestSimpleTuple();

  TestSimpleTupleGet();

  // TestTimer();

  SimpleLoop();

  TestTimeoutLoop();

  TestEvEpoll();

  return 0;
}