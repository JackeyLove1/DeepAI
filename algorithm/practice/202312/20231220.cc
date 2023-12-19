#include <atomic>
#include <bits/stdc++.h>
#include <bthread/bthread.h>
#include <bthread/countdown_event.h>
#include <bthread/mutex.h>
#include <butil/atomicops.h>
#include <butil/time.h>
#include <bvar/bvar.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <iostream>
#include <thread>

struct Arg {
  bthread::CountdownEvent event;
  butil::atomic<int> num_sig;
};

void* signaler(void* arg) {
  Arg* a = (Arg*)arg;
  a->num_sig.fetch_sub(1, butil::memory_order_relaxed);
  a->event.signal();
  return nullptr;
}

void CountdonwEventTest_sanity() {
  for (int n = 1; n < 10; ++n) {
    Arg a;
    a.num_sig = n;
    a.event.reset(n);
    for (int i = 0; i < n; ++i) {
      bthread_t tid;
      assert(0 == bthread_start_urgent(&tid, NULL, signaler, &a));
    }
    a.event.wait();
    assert(0 == a.num_sig.load(butil::memory_order_relaxed));
  }
  LOG(INFO) << "CountdonwEventTest_sanity Succeed";
}

void CountdonwEventTest_timed_wait() {
  bthread::CountdownEvent event;
  int rc = event.timed_wait(butil::milliseconds_from_now(100));
  assert(rc == ETIMEDOUT);
  event.signal();
  rc = event.timed_wait(butil::milliseconds_from_now(100));
  assert(rc == 0);
  bthread::CountdownEvent event2;
  event2.signal();
  rc = event2.timed_wait(butil::milliseconds_from_now(100));
  assert(rc == 0);
  LOG(INFO) << "CountdonwEventTest_timed_wait Succeed";
}

struct WorkerArgs {
  std::atomic<int> nums;
  bthread::CountdownEvent waiter;
};

void* bthread_work(void* arg) {
  WorkerArgs* worker_args = reinterpret_cast<WorkerArgs*>(arg);
  // std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
  worker_args->nums.fetch_add(1, std::memory_order_seq_cst);
  worker_args->waiter.signal();
  return nullptr;
}

void BthreadTest() {
  WorkerArgs args;
  const uint64_t NUM_THREADS = 1000;
  std::array<bthread_t, NUM_THREADS> bthreads{};
  args.waiter.reset(0);
  args.nums.store(0);
  for (auto i = 0u; i < NUM_THREADS; ++i) {
    args.waiter.add_count(1);
    if (bthread_start_background(&bthreads[i], nullptr, bthread_work, &args) !=
        0) {
      LOG(ERROR) << "Fail to start bthread, tid: " << bthreads[i];
      return;
    }
  }
  for (auto i = 0u; i < NUM_THREADS; ++i) {
    bthread_join(bthreads[i], nullptr);
  }
  args.waiter.wait();
  LOG(INFO) << "Counter: " << args.nums.load(std::memory_order_relaxed)
            << " Expected: " << NUM_THREADS;
}

struct MutexArg {
  bthread::Mutex mutex;
  int counter;
};

void* mutex_work(void* arg) {
  MutexArg* worker_args = reinterpret_cast<MutexArg*>(arg);
  std::unique_lock<bthread::Mutex> lock(worker_args->mutex);
  worker_args->counter++;
  return nullptr;
}

void MutexTest() {
  MutexArg args;
  const uint64_t NUM_THREADS = 1000;
  std::array<bthread_t, NUM_THREADS> bthreads{};
  args.counter = 0;
  for (auto i = 0u; i < NUM_THREADS; ++i) {
    if (bthread_start_background(&bthreads[i], nullptr, mutex_work, &args) !=
        0) {
      LOG(ERROR) << "Fail to start bthread, tid: " << bthreads[i];
      return;
    }
  }
  for (auto i = 0u; i < NUM_THREADS; ++i) {
    bthread_join(bthreads[i], nullptr);
  }
  LOG(INFO) << "Counter: " << args.counter << " Expected: " << NUM_THREADS;
}

void TimeCost() {
  butil::Timer t;
  t.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  t.stop();
  LOG(INFO) << "time cost: " << t.m_elapsed();
}

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  // FLAGS_log_dir = "./log";
  FLAGS_minloglevel = google::INFO;
  LOG(INFO) << "Hello, World!";
  bvar::Adder<int> var;
  var << 1;
  std::cout << var.get_value() << std::endl;
  FLAGS_logtostderr = 1;
  const int num_cookies = 10;
  LOG(INFO) << "num_cookies: " << num_cookies;
  //  DLOG(INFO) << "num_cookies: " << num_cookies;
  //  CHECK(num_cookies > 0) << "Check num_cookies: " << num_cookies;

  CountdonwEventTest_sanity();
  CountdonwEventTest_timed_wait();
  BthreadTest();
  MutexTest();
  TimeCost();
  return 0;
}