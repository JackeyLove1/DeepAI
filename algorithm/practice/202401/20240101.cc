#include <atomic>
#include <bits/stdc++.h>
#include <bthread/bthread.h>
#include <glog/logging.h>
#include <mutex>
#include <thread>

#define byte_alignment 64

using AgentId = int;
template <typename Agent>
class AgentGroup {
 public:
  using agent_type = Agent;
  struct ThreadBlock {
    inline Agent* get(size_t offset) {
      assert(offset < _agents.size());
      return _agents[offset];
    };

   private:
    std::vector<Agent> _agents;
  } __attribute__((aligned(byte_alignment)));

  inline static Agent create_new_agent() {
    std::unique_lock<std::mutex> lock_guard(_s_mutex);
    assert(_s_agent_kinds >= 0);
    AgentId agent_id = _s_agent_kinds++;
    return agent_id;
  }

  inline static Agent* get_or_create_tls_agent(Agent block_id) {
    if (_s_tls_block == nullptr) {
      _s_tls_block =
          std::make_unique<std::vector<std::unique_ptr<ThreadBlock>>>();
    }
    if (block_id > _s_tls_block->size()) {
      _s_tls_block->resize(std::max(block_id + 1, 32u));
    }
    ThreadBlock* tls_block = (*_s_tls_block)[block_id].get();
    if (tls_block == nullptr) {
      auto* new_tls_block = new (std::nothrow) ThreadBlock;
      if (__builtin_expect(new_tls_block == nullptr, 0)) {
        return nullptr;
      }
      tls_block = new_tls_block;
      (*_s_tls_block)[block_id].reset(new_tls_block);
    }
    return tls_block->get(block_id);
  }

 private:
  static std::mutex _s_mutex;
  static AgentId _s_agent_kinds;
  static thread_local std::unique_ptr<std::vector<ThreadBlock*>> _s_tls_block;
};

template <typename T>
std::mutex AgentGroup<T>::_s_mutex;

template <typename T>
AgentId AgentGroup<T>::_s_agent_kinds = 0;

template <typename T>
thread_local std::unique_ptr<std::vector<typename AgentGroup<T>::ThreadBlock*>>
    AgentGroup<T>::_s_tls_block = nullptr;

template <typename T>
class AgentCombiner {
 public:
  AgentCombiner() : _id(AgentGroup<T>::create_new_agent()) {}

 public:
  AgentId _id;
  std::list<std::vector<T>*> _agents;
  std::vector<T> _agent;
};

template <typename C>
using ContainerIter = decltype(std::begin(std::declval<C&>()));

#define PREDICT_FALSE(expr) (__builtin_expect(false || (expr), false))
#define PREDICT_TRUE(expr) (__builtin_expect(false || (expr), true))

void TestDefine() {
  const std::string s1 = "Hello";
  if (PREDICT_TRUE(s1 == "Hello")) {
    LOG(INFO) << "Equal";
  }
  if (PREDICT_FALSE(s1 == "World")) {
  } else {
    LOG(INFO) << "Not Equal";
  }
}

// call once
class once_flag {
 public:
  constexpr once_flag() : control_(false) {}
  once_flag(const once_flag&) = delete;
  once_flag& operator=(const once_flag&) = delete;
  once_flag&& operator=(once_flag&&) = delete;
  friend std::atomic_bool* ControlWord(once_flag* flag) {
    return &flag->control_;
  };

 private:
  std::atomic_bool control_;
};

enum {
  kOnceInit = 0,
  kOnceRunning = 1,
  kOnceWaiter = 2,
  kOnceDone = 3,
};

template <typename Callable, typename... Args>
void call_once(once_flag& flag, Callable&& func, Args&&... args) {
  std::atomic_bool* control = ControlWord(&flag);
  if (!control->load(std::memory_order_acquire)) {
    control->store(true, std::memory_order_seq_cst);
    LOG(INFO) << "Doing...";
    std::invoke(std::forward<Callable>(func), std::forward<Args>(args)...);
  }
}

const uint32_t num_bthreads = 2000;
const uint32_t loops = 100'000;

struct WorkArgs {
  explicit WorkArgs() : count(std::make_unique<std::atomic<int>>(0)), flag(std::make_unique<once_flag>()){}
  std::unique_ptr<std::atomic<int>> count{nullptr};
  std::unique_ptr<once_flag> flag;
};

void* CallWork(void * arg){
  auto* work_arg = reinterpret_cast<WorkArgs*>(arg);
  call_once(*work_arg->flag, [](WorkArgs* arg) {
    for (auto loop = 0u; loop < 10u; ++loop) {
      arg->count->fetch_and(1, std::memory_order_relaxed);
      LOG(INFO) <<"Thread: " << std::this_thread::get_id() << " execute, value: " << arg->count->load(std::memory_order_release);
    }
    LOG(INFO) <<"Thread: " << std::this_thread::get_id() << " done, value: " << arg->count->load(std::memory_order_release);
  }, work_arg);
  return nullptr;
}

void TestCallOnce() {
  WorkArgs args{};
  std::array<bthread_t, num_bthreads> bthreads{};

  for (uint32_t i = 0; i < num_bthreads; ++i) {
    bthread_start_background(
        &bthreads[i], nullptr,
        CallWork,
        &args);
  }
  for (uint32_t i = 0; i < num_bthreads; ++i) {
    bthread_join(bthreads[i], nullptr);
  }
  usleep(100);
  LOG(INFO) << "value: " << args.count->load(std::memory_order_seq_cst)
            << " Expected: " << loops;
}

int main() {
  TestDefine();

  TestCallOnce();

  return 0;
}