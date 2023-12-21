#include <bits/stdc++.h>
#include <brpc/server.h>
#include <bthread/bthread.h>
#include <bthread/mutex.h>
#include <mutex>
#include <thread>

using std::cout;

std::mutex mutex1, mutex2, mutex3;

void FuncA() {
  std::lock_guard<std::mutex> guard1(mutex1);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::lock_guard<std::mutex> guard2(mutex2);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void FuncB() {
  std::lock_guard<std::mutex> guard2(mutex2);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::lock_guard<std::mutex> guard3(mutex3);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void FuncC() {
  std::lock_guard<std::mutex> guard3(mutex3);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::lock_guard<std::mutex> guard1(mutex1);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void TestPthreadDeadLock() {
  std::thread A(FuncA);
  std::thread B(FuncB);
  std::thread C(FuncC);

  std::this_thread::sleep_for(std::chrono::seconds(5));

  if (A.joinable()) {
    A.join();
  }
  if (B.joinable()) {
    B.join();
  }
  if (C.joinable()) {
    C.join();
  }
  cout << "hello\n";
}

bthread_mutex_t bmutex1;
bthread_mutex_t bmutex2;

void* myfunction1(void* dummy) {
  bthread_mutex_lock(&bmutex1);
  sleep(1);
  bthread_mutex_lock(&bmutex2);
  usleep(1000);
  bthread_mutex_unlock(&bmutex1);
  bthread_mutex_unlock(&bmutex2);

  return nullptr;
}

void* myfunction2(void* dummy) {
  bthread_mutex_lock(&bmutex2);
  sleep(1);
  bthread_mutex_lock(&bmutex1);
  usleep(1000);
  bthread_mutex_unlock(&bmutex2);
  bthread_mutex_unlock(&bmutex1);

  return nullptr;
}


void TestBthreadDeadLock() {
  bthread_t thread_id1, thread_id2;

  bthread_mutex_init(&bmutex1, nullptr);
  bthread_mutex_init(&bmutex2, nullptr);

  bthread_start_background(&thread_id1, nullptr, &myfunction1, nullptr);
  bthread_start_background(&thread_id2, nullptr, &myfunction2, nullptr);

  bthread_join(thread_id1, nullptr);
  bthread_join(thread_id2, nullptr);

  bthread_mutex_destroy(&bmutex1);
  bthread_mutex_destroy(&bmutex2);
  std::cout << "Hello, Bthread Lock Done!" << "\n";
}

int RunFunctionInBthread(const std::function<void()>& fn) {
  bthread_t tid;
  auto func_ptr = std::make_unique<std::function<void()>>(fn);
  if (bthread_start_background(
          &tid, &BTHREAD_ATTR_NORMAL,
          [](void* p) -> void* {
            std::unique_ptr<std::function<void()>> fn(reinterpret_cast<std::function<void()>*>(p));
            (*fn)();
            return nullptr;
          },
          func_ptr.release()) != 0) {
    return -1;
  }
  return 0;
}



int main() {
  // TestPthreadDeadLock();
  // TestBthreadDeadLock();
  return 0;
}