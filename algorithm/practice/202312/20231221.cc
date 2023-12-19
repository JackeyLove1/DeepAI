#include <bits/stdc++.h>
#include <brpc/channel.h>
#include <brpc/controller.h>
#include <brpc/server.h>
#include <bthread/bthread.h>
#include <bthread/countdown_event.h>
#include <bthread/execution_queue.h>
#include <butil/time.h>
#include <glog/logging.h>

struct LongIntTask {
  long value;
  bthread::CountdownEvent* event;

  LongIntTask(long v) : value(v), event(nullptr) {}
  LongIntTask(long v, bthread::CountdownEvent* e) : value(v), event(e) {}
  LongIntTask() : value(0), event(nullptr) {}
};

int add(void* meta, bthread::TaskIterator<LongIntTask>& iter) {
  bool stopped = iter.is_queue_stopped();
  auto* result = static_cast<int64_t*>(meta);
  for (; iter; ++iter) {
    *result += iter->value;
    if (iter->event) {
      iter->event->signal();
    }
  }
  return 0;
}

void test_single_thread(bool use_pthread = false) {
  int64_t result = 0;
  int64_t expected_result = 0;
  const int N = 1000;
  bthread::ExecutionQueueId<LongIntTask> queue_id{};
  bthread::ExecutionQueueOptions options;
  options.use_pthread = false;
  bthread::execution_queue_start(&queue_id, &options, add, &result);
  for (int i = 0; i < N; ++i) {
    expected_result += i;
    bthread::execution_queue_execute(queue_id, LongIntTask(i));
  }
  bthread::execution_queue_stop(queue_id);
  bthread::execution_queue_execute(queue_id, 0);
  bthread::execution_queue_join(queue_id);
  CHECK_EQ(expected_result, result);
  LOG(INFO) << "Result: " << result << " Expected: " << expected_result;
}

DEFINE_string(TestString, "Hello, World!", "Test String");
DEFINE_int32(TestInt, 100, "Test Int");
DEFINE_double(TestDouble, 3.1415926, "Test Double");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  LOG(INFO) << "Hello, Meta!";
  brpc::Channel channel;
  brpc::ChannelOptions options;
  options.protocol = brpc::PROTOCOL_HTTP;
  options.max_retry = 3;
  options.timeout_ms = 2000;
  if (channel.Init("https://www.baidu.com", "", &options) != 0) {
    LOG(ERROR) << "Faield to init channel";
    return -1;
  }

  brpc::Controller cntl;
  cntl.http_request().uri() = "https://www.baidu.com";
  channel.CallMethod(nullptr, &cntl, nullptr, nullptr, nullptr);
  if (cntl.Failed()) {
    LOG(ERROR) << "Failed to call method: " << cntl.ErrorText();
    // return -1;
  } else {
    LOG(INFO) << "Context: " << cntl.response_attachment().to_string();
  }

  test_single_thread();

  brpc::StartDummyServerAt(8888);
  while (true) {
    sleep(1);
  }
}