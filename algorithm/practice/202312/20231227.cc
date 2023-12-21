#include <bits/stdc++.h>
#include <bthread/bthread.h>
#include <bthread/butex.h>
#include <bthread/condition_variable.h>
#include <bthread/execution_queue.h>
#include <bthread/mutex.h>
#include <butil/base64.h>
#include <butil/time.h>
#include <bvar/bvar.h>
#include <glog/logging.h>

void TestPthread() {
  const int counter_loop = 1'000'000;
  const uint32_t num_threads = 32;
  std::vector<std::thread> threads{};
  threads.reserve(num_threads);
  butil::Timer timer1, timer2;

  timer1.start();
  std::atomic<int> atomic_counter(0);
  for (auto i = 0u; i < num_threads; i++) {
    threads.emplace_back([&]() {
      for (int i = 0; i < counter_loop; ++i) {
        atomic_counter.fetch_add(1, std::memory_order_relaxed);
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  timer1.stop();
  LOG(INFO) << "Atomic Counter Value: " << atomic_counter.load();
  LOG(INFO) << "Atomic Add Cost: " << timer1.m_elapsed() << " ms";

  threads.clear();

  timer2.start();
  bvar::Adder<int> bvar_counter;
  for (auto i = 0u; i < num_threads; i++) {
    threads.emplace_back([&]() {
      for (int i = 0; i < counter_loop; ++i) {
        bvar_counter << 1;
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  timer2.stop();
  LOG(INFO) << "Bvar Counter Value: " << bvar_counter.get_value();
  LOG(INFO) << "Bvar Add Cost: " << timer2.m_elapsed() << " ms";
}

// __thread int tls_int = 0;

void TestTls() {
  thread_local int tls_int = 0;
  const int counter_loop = 1'000'000;
  const uint32_t num_threads = 32;
  std::vector<std::thread> threads{};
  threads.reserve(num_threads);
  butil::Timer timer1;

  timer1.start();
  std::atomic<int> tls_sum(0);
  for (auto i = 0u; i < num_threads; i++) {
    threads.emplace_back([&, i]() {
      LOG(INFO) << "Thread: " << std::this_thread::get_id() << " idx: " << i
                << " TLS address: " << static_cast<void*>(&tls_int);
      for (int loop = 0; loop < counter_loop; ++loop) {
        ++tls_int;
      }
      tls_sum += tls_int;
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  timer1.stop();
  LOG(INFO) << "TLS Counter Sum: " << tls_sum.load();
  LOG(INFO) << "TLS Add Cost: " << timer1.m_elapsed() << " ms";
}

void TestSingle() {
  const int counter_loop = 1'000'000;
  std::atomic<int> atomic_counter{0};
  butil::Timer timer1, timer2;
  timer1.start();
  std::thread t([&]() {
    for (int i = 0; i < counter_loop; ++i) {
      atomic_counter.fetch_add(1, std::memory_order_relaxed);
    }
  });
  t.join();
  timer1.stop();
  LOG(INFO) << "Single Counter Sum: " << atomic_counter.load();
  LOG(INFO) << "Single Add Cost: " << timer1.m_elapsed() << " ms";
}

void TestFunction() {
  std::map<int, int> mp;
  std::function<void(int, int)> func1 = [&](int a, int b) {
    mp[a] = b;
    std::cout << "Hello, World!"
              << " a:" << a << " b:" << b << std::endl;
  };
  std::function<void(int, int, int)> func2 = [&](int a, int b, int c) {
    mp[a] = b;
    std::cout << "Hello, World!"
              << " a:" << a << " b:" << b << " c:" << c << std::endl;
  };
  std::function<int(int, int, int)> func3 = [&](int a, int b, int c) {
    return a + b + c;
  };
  LOG(INFO) << "sizeof(func1): " << sizeof(func1);
  LOG(INFO) << "sizeof(func2): " << sizeof(func2);
  LOG(INFO) << "sizeof(func3): " << sizeof(func3);
  LOG(INFO) << "sizeof(void*): " << sizeof(void*);
}

std::ostream& operator<<(std::ostream& ostr, const std::list<int>& list) {
  for (auto& i : list) ostr << ' ' << i;
  return ostr;
}

void TestListSplice() {
  std::list<int> list1{1, 2, 3, 4, 5};
  std::list<int> list2{10, 20, 30, 40, 50};

}

using CallBack = std::function<void()>;

class AbstractThreadPool {
 public:
  virtual ~AbstractThreadPool() = default;
  virtual bool AddTask(const CallBack& func) = 0;
  virtual void WaitForIdle() = 0;
  virtual void Terminate() = 0;
  virtual std::vector<int> GetAllThreadIds() { return {}; }
};

struct Task {
  CallBack func;
};

struct ThreadContext {
  using TaskList = std::list<Task>;
  using TaskListIter = TaskList::iterator;
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  TaskList pending_list_;
  TaskList done_list_;
  size_t thread_index_;
  bool GetPendingTask(TaskList& other_tasks) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (pending_list_.empty()) {
      return false;
    }
    other_tasks.splice(other_tasks.end(), pending_list_);
    return true;
  }
} __attribute__((aligned(64)));

class BaseThreadPool : public AbstractThreadPool {
 public:
  explicit BaseThreadPool(int num_threads = -1,
                          const std::string& name = "BaseThreadPool") {}

 private:
  void AddTaskInternal(CallBack& func, int dispatch_key, bool fixed_thread) {}

  void WorkRoutine() {}

 private:
  std::vector<ThreadContext> thread_contexts_;
  std::vector<std::atomic<bool>> thread_alive_bitmap_;
  size_t num_threads_;
  size_t num_alive_threads_;
  std::atomic<size_t> num_busy_threads_;
  std::mutex exit_lock_;
  std::condition_variable_any exit_cond_;
  bool exit_;
};

int main() {
  // TestPthread();

  // TestTls();

  // TestSingle();

  TestFunction();

  TestListSplice();

  return 0;
}