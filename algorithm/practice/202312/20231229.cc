#include <bits/stdc++.h>
#include <bthread/bthread.h>
#include <bthread/butex.h>
#include <bthread/condition_variable.h>
#include <bthread/execution_queue.h>
#include <bthread/mutex.h>
#include <butil/base64.h>
#include <butil/time.h>
#include <bvar/bvar.h>
#include <cstddef>
#include <glog/logging.h>
#include <iostream>
#include <memory>
#include <pthread.h>

#include "butil/atomicops.h"
#include "butil/macros.h"
#include "butil/time.h"
#include "bvar/detail/agent_group.h"

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
  CallBack callback;
};

struct ThreadContext {
  using TaskList = std::list<Task>;
  using TaskListIter = TaskList::iterator;
  explicit ThreadContext()
      : mutex_ptr_(std::make_unique<std::mutex>()),
        cv_ptr_(std::make_unique<std::condition_variable_any>()){};
  TaskList pending_list_{};
  TaskList done_list_{};
  size_t thread_index_{0};
  std::unique_ptr<std::thread> thread_ptr_{nullptr};
  std::unique_ptr<std::mutex> mutex_ptr_{nullptr};
  std::unique_ptr<std::condition_variable_any> cv_ptr_{nullptr};
  bool GetPendingTask(TaskList& other_tasks) {
    std::unique_lock<std::mutex> lock(*mutex_ptr_);
    if (pending_list_.empty()) {
      return false;
    }
    other_tasks.splice(other_tasks.end(), pending_list_);
    return true;
  }
} __attribute__((aligned(64)));

class BaseThreadPool : public AbstractThreadPool {
 public:
  explicit BaseThreadPool(size_t num_threads,
                          const std::string& name = "BaseThreadPool")
      : exit_(false),
        num_threads_(num_threads),
        thread_alive_bitmap_(num_threads) {
    if (num_threads_ <= 0) {
      num_threads_ = std::thread::hardware_concurrency();
    }
    for (size_t i = 0; i < num_threads_; ++i) {
      thread_contexts_.emplace_back();
      std::function<void(ThreadContext*)> work = [&](ThreadContext* context) {
        WorkRoutine(context);
      };
      auto work_bind = std::bind(work, &thread_contexts_.back());
      auto thread_ptr = std::make_unique<std::thread>(work_bind);
      thread_contexts_[i].thread_ptr_ = std::move(thread_ptr);
      thread_contexts_[i].thread_index_ = i;
      thread_alive_bitmap_[i].store(false, std::memory_order_relaxed);
    }
  }

 private:
  void AddTaskInternal(CallBack& callback, int dispatch_key) {
    int dispatch_index = dispatch_key % num_threads_;
    auto& context = thread_contexts_[dispatch_index];
    context.pending_list_.emplace_back(Task{callback});
    context.cv_ptr_->notify_one();
  }

  void WorkRoutine(ThreadContext* context) {
    ThreadContext::TaskList tasks;
    while (true) {
      bool continued = context->GetPendingTask(tasks);
      thread_alive_bitmap_[context->thread_index_].store(
          true, std::memory_order_release);
      num_busy_threads_.fetch_add(1, std::memory_order_acquire);
      for (auto& task : tasks) {
        task.callback();
      }
      if (!continued) {
        break;
      }
    }
    thread_alive_bitmap_[context->thread_index_].store(
        false, std::memory_order_release);
    if (num_busy_threads_.fetch_sub(1, std::memory_order_acquire) == 0) {
      exit_cond_.notify_all();
    }
  }

 private:
  bool exit_;
  size_t num_threads_;
  size_t num_alive_threads_{};
  std::vector<ThreadContext> thread_contexts_;
  std::vector<std::atomic<bool>> thread_alive_bitmap_;
  std::atomic<size_t> num_busy_threads_{};
  std::mutex exit_lock_;
  std::condition_variable_any exit_cond_;
};

void TestBvarProcess() {
  bvar::Adder<int> badder1;
  bvar::Adder<int> badder2;
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    std::thread t([&]() {
      badder1 << 1;
      badder2 << 1;
    });
    threads.push_back(std::move(t));
  }
  for (auto&& t : threads) {
    t.join();
  }
  LOG(INFO) << "badder1: " << badder1.get_value();
  LOG(INFO) << "badder2: " << badder2.get_value();
}

using namespace bvar::detail;
const size_t OPS_PER_THREAD = 2000;
class AgentGroupTest {
 public:
  typedef butil::atomic<uint64_t> agent_type;

  static void* thread_counter(void* arg) {
    int id = (int)((long)arg);
    agent_type* item = AgentGroup<agent_type>::get_or_create_tls_agent(id);
    if (item == NULL) {
      return NULL;
    }
    butil::Timer timer;
    timer.start();
    for (size_t i = 0; i < OPS_PER_THREAD; ++i) {
      agent_type* element = AgentGroup<agent_type>::get_or_create_tls_agent(id);
      uint64_t old_value = element->load(butil::memory_order_relaxed);
      uint64_t new_value;
      do {
        new_value = old_value + 2;
      } while (__builtin_expect(
          !element->compare_exchange_weak(old_value, new_value,
                                          butil::memory_order_relaxed,
                                          butil::memory_order_relaxed),
          0));
    }
    timer.stop();
    return (void*)(timer.n_elapsed());
  }
};

void TestBvarTest2() {
  long id = AgentGroup<AgentGroupTest::agent_type>::create_new_agent();
  LOG(INFO) << "agent id: " << id;
  pthread_t threads[24];
  for (size_t i = 0; i < ARRAY_SIZE(threads); ++i) {
    pthread_create(&threads[i], NULL, &AgentGroupTest::thread_counter,
                   (void*)id);
  }
  long totol_time = 0;
  for (size_t i = 0; i < ARRAY_SIZE(threads); ++i) {
    void* ret;
    pthread_join(threads[i], &ret);
    totol_time += (long)ret;
  }
  LOG(INFO) << "ThreadAgent takes "
            << totol_time / (OPS_PER_THREAD * ARRAY_SIZE(threads));
  totol_time = 0;
}

void TestBvarSimple() {
  bvar::Adder<int> badder1;
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    std::thread t([&]() {
      badder1 << 1;
    });
    threads.push_back(std::move(t));
  }
  for (auto&& t : threads) {
    t.join();
  }
  LOG(INFO) << "badder1: " << badder1.get_value();
}

int main() {
  // TestPthread();

  // TestTls();

  // TestSingle();

  TestFunction();

  TestListSplice();

  // TestBvarProcess();

  // TestBvarTest2();

  TestBvarSimple();

  return 0;
}