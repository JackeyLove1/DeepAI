#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <bits/stdc++.h>
#include <boost/filesystem.hpp>
#include <brpc/channel.h>
#include <bthread/bthread.h>
#include <butil/iobuf.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <bvar/bvar.h>
#include <bvar/multi_dimension.h>
#include <chrono>
#include <cxxabi.h>
#include <fstream>
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <type_traits>
#include <typeinfo>

bvar::MultiDimension<bvar::Adder<int>> g_request_count("request_count",
                                                       {"idc", "method",
                                                        "status"});
// bvar::MultiDimension<bvar::Adder<int> > g_request_count("foo_bar",
// "request_count", {"idc", "method", "status"});

int process_request(const std::list<std::string>& request_label) {
  bvar::Adder<int>* addr = g_request_count.get_stats(request_label);
  if (!addr) {
    return -1;
  }
  *addr << 1 << 2 << 3;
  LOG(INFO) << "addr=" << *addr;
  return 0;
}

void TestIOBufAndStringPerformance() {
  butil::IOBufBuilder builder;
  butil::IOBuf buf;
  butil::Timer timer1, timer2, timer3;
  const size_t N = 2000;
  const std::string s = "Hello World";

  timer1.start();
  for (size_t i = 0; i < N; ++i) {
    builder << s;
  }
  builder.move_to(buf);
  // std::string str3 = buf.to_string();

  timer1.stop();
  LOG(INFO) << "IOBuf Cost: " << timer1.m_elapsed() << "ms";

  timer2.start();
  std::string str;
  for (size_t i = 0; i < N; ++i) {
    str += s;
  }
  timer2.stop();
  LOG(INFO) << "String Cost: " << timer2.m_elapsed() << "ms";

  timer3.start();
  std::string str2;
  str2.reserve(N * s.size());
  for (size_t i = 0; i < N; ++i) {
    str += s;
  }
  timer3.stop();
  LOG(INFO) << "String Reserve Cost: " << timer3.m_elapsed() << "ms";
}

int RunFunctionInBthread(const std::function<void()>& fn) {
  bthread_t tid;
  auto func_ptr = std::make_unique<std::function<void()>>(fn);
  if (bthread_start_background(
          &tid, &BTHREAD_ATTR_NORMAL,
          [](void* p) -> void* {
            std::unique_ptr<std::function<void()>> fn(
                reinterpret_cast<std::function<void()>*>(p));
            (*fn)();
            return nullptr;
          },
          func_ptr.release()) != 0) {
    return -1;
  }
  return 0;
}

void TestRunFunctionInBthread() {
  auto fn = []() { LOG(INFO) << "Hello World!"; };
  RunFunctionInBthread(fn);
  bthread_usleep(1000);
  LOG(INFO) << "Done!";
}

void TestBoostFileSystem() {
  using namespace boost::filesystem;
  path p("/tmp");
  LOG(INFO) << p.string();
  LOG(INFO) << p.parent_path().string();
  LOG(INFO) << p.filename().string();
  LOG(INFO) << p.stem().string();
}

std::string GetCurrentPath() {
  return boost::filesystem::current_path().string();
}

bool Exists(const std::string& path) { return boost::filesystem::exists(path); }

bool IsDirectory(const std::string& path) {
  return boost::filesystem::is_directory(path);
}

bool IsFile(const std::string& path) {
  return boost::filesystem::is_regular_file(path);
}

bool RemoveFile(const std::string& path) {
  return boost::filesystem::remove(path);
}

void RenameFile(const std::string& old_path, const std::string& new_path) {
  boost::filesystem::rename(old_path, new_path);
}

bool CreateDirectory(const std::string& path) {
  return boost::filesystem::create_directory(path);
}

bool RemoveDirectory(const std::string& path) {
  return boost::filesystem::remove_all(path);
}

int CreateTestDirectory(int num_dirs = 5, int num_files = 10,
                        const std::string& base_dir = ":local") {
  auto dir = boost::filesystem::path(GetCurrentPath()) / base_dir;
  const std::string content = "Hello, World!";
  bool result = true;
  if (!boost::filesystem::exists(dir)) {
    result = boost::filesystem::create_directories(dir);
  }
  if (!result) {
    LOG(ERROR) << "Fail to create dir: " << dir.string();
    return -1;
  } else {
    LOG(INFO) << "Success to create dir: " << dir.string();
  }

  // create sub dir
  for (int i = 0; i < num_dirs; ++i) {
    std::string dir_name = "dir" + std::to_string(i);
    boost::filesystem::path dir_path =
        boost::filesystem::path(base_dir) / dir_name;

    if (boost::filesystem::create_directories(dir_path)) {
      for (int j = 0; j < num_files; ++j) {
        std::string file_name = "file" + std::to_string(j);
        boost::filesystem::path file_path = dir_path / file_name;
        std::ofstream file(file_path.string());
        file << content;
        file.close();

        if (boost::filesystem::exists(file_path)) {
          LOG(INFO) << "Succeed to Create Directory: " << dir_path;
        } else {
          LOG(ERROR) << "Failed to Create Directory: " << dir_path;
          return -1;
        }
      }
    }
  }
  return 0;
}

void TestCreateDir() {
  // create base dir
  CreateTestDirectory();
}

template <typename T1, typename T2>
void TestType(T1, T2) {
  if (!std::is_same<T1, T2>::value) {
    LOG(INFO) << "type is same";
  } else {
    LOG(INFO) << "type is different";
  }
}

void TestTypeFunc() {
  TestType("hello", 1);
  TestType(1, 1);
  TestType(1, 1.0);
  TestType(1.0, 1);
  TestType(1.0, 1.0);
}

template <typename T>
constexpr bool is_const_v = std::is_const<T>::value;

void TestConstValue() {
  LOG(INFO) << "is_const_v<int>: " << is_const_v<int>;
  LOG(INFO) << "is_const_v<const int>: " << is_const_v<const int>;
}

template <typename T>
typename std::enable_if_t<(sizeof(T) > 4), T> foo() {
  return T();
}

void TestEnableIfFoo() {
  // LOG(INFO) << "Int: " << foo<int>();
  LOG(INFO) << "Double: " << foo<double>();
  LOG(INFO) << "String: " << foo<std::string>();
}

template <typename T, std::size_t L1, std::size_t L2>
void CompareArray(T (&arg1)[L1], T (&arg2)[L2]) {
  T* pa = arg1;
  T* pb = arg2;
  if (L1 != L2) {
    LOG(INFO) << "array length is not equal";
    return;
  }
  for (auto i = 0u; i < L1; ++i) {
    if (pa[i] != pb[i]) {
      LOG(INFO) << "array content is not equal, index: " << i;
      return;
    }
  }
  LOG(INFO) << "Array is equal";
}

template <typename T1, typename T2,
          typename = std::enable_if<
              std::is_array<T1>::value && std::is_array<T2>::value &&
              std::is_same<typename std::remove_all_extents<T1>::type,
                           typename std::remove_all_extents<T2>::type>::value &&
              std::extent<T1>::value == std::extent<T2>::value>>
void CompareArray(T1&& arg1, T2&& arg2) {
  for (auto i = 0u; i < std::extent<T1>::value; ++i) {
    if (arg1[i] != arg2[i]) {
      std::cout << "Array content is not equal, index: " << i << std::endl;
      return;
    }
  }
  std::cout << "Array is equal" << std::endl;
}

void TestArray() {
  int a[3] = {1, 2, 3};
  int b[3] = {1, 2, 3};
  int d[4] = {0, 1, 2, 3};
  int e[3] = {0, 1, 2};
  CompareArray(a, b);
  CompareArray(a, d);
  CompareArray(a, e);

  std::array<int, 3> arr1 = {1, 2, 3};
  std::array<double, 3> arr2 = {1.0, 2.0, 3.0};
  std::array<int, 4> arr3 = {1, 2, 3, 4};
  std::array<int, 3> arr4 = {2, 3, 4};
  CompareArray(arr1, arr2);
  CompareArray(arr1, arr3);
  CompareArray(arr1, arr4);
}

int main() {
  brpc::Channel channel;
  brpc::ChannelOptions options;
  options.protocol = brpc::PROTOCOL_HTTP;
  options.timeout_ms = 2000;
  options.max_retry = 3;
  const std::string url = "www.baidu.com";
  if (channel.Init(url.c_str(), "", &options)) {
    LOG(ERROR) << "Fail to initialize channel";
    return -1;
  }

  brpc::Controller cntl;
  cntl.http_request().uri() = url;
  cntl.http_request().set_method(brpc::HTTP_METHOD_GET);
  cntl.request_attachment().append("");
  channel.CallMethod(nullptr, &cntl, nullptr, nullptr, nullptr);
  if (cntl.Failed()) {
    LOG(ERROR) << "Fail to call " << cntl.http_request().uri() << ": "
               << cntl.ErrorText();
  } else {
    LOG(INFO) << "Status code: " << cntl.http_response().status_code();
    LOG(INFO) << "Response body: " << cntl.response_attachment().to_string();
  }

  TestIOBufAndStringPerformance();

  TestRunFunctionInBthread();

  TestBoostFileSystem();

  // TestCreateDir();

  LOG(INFO) << "Current path: " << GetCurrentPath();

  TestTypeFunc();

  TestConstValue();

  TestEnableIfFoo();

  TestArray();

  return 0;
}
