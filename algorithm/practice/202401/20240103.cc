#include <atomic>
#include <bits/stdc++.h>
#include <boost/filesystem.hpp>
#include <bthread/bthread.h>
#include <glog/logging.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <type_traits>

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
/*
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
  explicit WorkArgs()
      : count(std::make_unique<std::atomic<int>>(0)),
        flag(std::make_unique<once_flag>()) {}
  std::unique_ptr<std::atomic<int>> count{nullptr};
  std::unique_ptr<once_flag> flag;
};

void* CallWork(void* arg) {
  auto* work_arg = reinterpret_cast<WorkArgs*>(arg);
  call_once(
      *work_arg->flag,
      [](WorkArgs* arg) {
        for (auto loop = 0u; loop < 10u; ++loop) {
          arg->count->fetch_and(1, std::memory_order_relaxed);
          LOG(INFO) << "Thread: " << std::this_thread::get_id()
                    << " execute, value: "
                    << arg->count->load(std::memory_order_release);
        }
        LOG(INFO) << "Thread: " << std::this_thread::get_id()
                  << " done, value: "
                  << arg->count->load(std::memory_order_release);
      },
      work_arg);
  return nullptr;
}

void TestCallOnce() {
  WorkArgs args{};
  std::array<bthread_t, num_bthreads> bthreads{};

  for (uint32_t i = 0; i < num_bthreads; ++i) {
    bthread_start_background(&bthreads[i], nullptr, CallWork, &args);
  }
  for (uint32_t i = 0; i < num_bthreads; ++i) {
    bthread_join(bthreads[i], nullptr);
  }
  usleep(100);
  LOG(INFO) << "value: " << args.count->load(std::memory_order_seq_cst)
            << " Expected: " << loops;
}


template <typename Callable, typename... Args>
auto Call(Callable& func, Args&&... args) {
  return std::invoke(std::forward<Callable>(func), std::forward<Args>(args)...);
}

void TestCall() {
  auto add = [](int a, int b) { return a + b; };
  LOG(INFO) << "Call Add: " << Call(add, 1, 2) << "Expect: 3";
  return;
}
*/
void TestFileSystem() {
  boost::filesystem::path p1;
  p1.string();
}

// template <template <typename...> class ContainerType, typename ValueType>
// std::ostream& operator<<(std::ostream& os,
//                          ContainerType<ValueType> const& container) {
//   os << "{";
//   for (const auto& value : container) {
//     os << value << ", ";
//   }
//   os << "}";
//   return os;
// }
//
// template <typename T, std::size_t N>
// std::ostream& operator<<(std::ostream& os, std::array<T, N> const& container)
// {
//   os << "{";
//   for (const auto& value : container) {
//     os << value << ", ";
//   }
//   os << "}";
//   return os;
// }

// template <typename T, typename U>
// std::ostream& operator<<(std::ostream& os, std::pair<T, U> const& pair) {
//   return os << "{" << pair.first << ", " << pair.second << "}";
// }
//
// template <template <typename...> class ContainerType, typename ValueType,
//           typename std::enable_if_t<!std::is_same<
//               ValueType, std::pair<typename ValueType::first_type,
//                                    typename ValueType::second_type>>::value>*
//                                    =
//               nullptr>
// std::ostream& operator<<(std::ostream& os,
//                          ContainerType<ValueType> const& container) {
//   static_assert(has_const_iterator<ContainerType<ValueType>>::value,
//                 "Not A Container!");
//   os << "{";
//   for (const auto& value : container) {
//     os << value << ", ";
//   }
//   os << "}";
//   return os;
// }

// template <class A, class B>
// std::ostream& operator<<(std::ostream& os, const std::pair<A, B>& p) {
//   return os << "{" << p.first << ", " << p.second << "}\n";
// }
//
//// Enable this overload only if T is not a std::pair.
// template <template <class...> class Z, class U,
//           std::enable_if_t<!std::is_same<
//               U, std::pair<typename U::first_type,
//                            typename U::second_type>>::value>* = nullptr>
// std::ostream& operator<<(std::ostream& os, const Z<U>& values) {
//   // Check if there's a const_iterator for Z<U> to determine whether it's a
//   // container.
//   static_assert(has_const_iterator<Z<U>>::value, "Not a container");
//
//   for (const auto& value : values) {
//     os << value << ", ";
//   }
//   os << "}\n";
//   return os;
// }

// template <typename Key, template <typename...> class Value, typename
// ValueType> std::ostream& operator<<(std::ostream& os,
//                          std::pair<Key, Value<ValueType>> const& pair) {
//   return os << "{" << pair.first << ", " << pair.second << "}";
// }
//
// template <template <typename...> class KeyContainer, typename KeyType,
//           template <typename...> class ValueContainer, typename ValueType>
// std::ostream& operator<<(
//     std::ostream& os,
//     std::pair<KeyContainer<KeyType>, ValueContainer<ValueType>> const& pair)
//     {
//   return os << "{" << pair.first << ", " << pair.second << "}";
// }

// template <template <typename...> class ContainerType, typename KeyType,
//           template <typename...> class ValueContainer, typename... ValueType>
// std::ostream& operator<<(
//     std::ostream& os,
//     ContainerType<KeyType, ValueContainer<ValueType...>> const& container) {
//   os << "{";
//   for (const auto& value : container) {
//     os << value << ", ";
//   }
//   os << "}";
//   return os;
// }

struct S {
  std::string name;
  int age;
  friend std::ostream& operator<<(std::ostream& os, const S& s) {
    return os << s.name << " " << s.age;
  }
};

void TestPrintContainer() {
  //  std::vector<int> vec = {1, 2, 3};
  //  LOG(INFO) << vec;
  //  std::array<int, 3> arr = {4, 5, 6};
  //  LOG(INFO) << arr;
  //  std::pair<int, int> p = {7, 8};
  //  LOG(INFO) << p;
  //  std::pair<std::string, std::string> p2 = {"k", "v"};
  //  LOG(INFO) << p2;
  //  std::map<int, int> m1;
  //  m1[9] = 10;
  //  LOG(INFO) << m1;
  //  std::map<int, std::pair<int, int>> m2;
  //  m2[101] = {9, 10};
  //  LOG(INFO) << m2;
  //  std::map<int, std::vector<int>> m3;
  //  m3[102] = {11, 12, 13};
  //  LOG(INFO) << m3;
  //  std::map<std::string, int> m4;
  //  m4["key1"] = 123;
  //  LOG(INFO) << m4;
}

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
struct has_const_iterator<T, void_t<typename T::const_iterator>>
    : public std::true_type {};

template <typename T>
class GLogPolicy {
 public:
  static void print(T const& value) { LOG(INFO) << value; }
};

template <typename ContainerType,
          template <typename...> class Policy = GLogPolicy,
          typename = std::enable_if_t<has_iterator<ContainerType>::value>>
auto LOGContainer(ContainerType const& container) {
  std::for_each(container.begin(), container.end(), [](const auto& value) {
    Policy<decltype(value)>::print(value);
  });
}

void TestLogContainer() {
  std::vector<int> v1 = {1, 2, 3};
  LOGContainer(v1);
  std::vector<std::string> v2 = {"1", "2", "3"};
  LOGContainer(v2);
  std::array<std::string, 3> a1 = {"4", "5", "6"};
  LOGContainer(a1);
  std::vector<std::string> v3 = {"123", "456", "789"};
  LOGContainer(v3);
}

bool CompareFiles(const boost::filesystem::path& p1,
                  const boost::filesystem::path& p2) {
  if (boost::filesystem::file_size(p1) != boost::filesystem::file_size(p2))
    return false;
  std::ifstream f1(p1.string(), std::ios::binary);
  std::ifstream f2(p2.string(), std::ios::binary);

  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

void TraverseDirectory(const boost::filesystem::path& path,
                       std::vector<std::string>& filenames) {
  namespace fs = boost::filesystem;
  if (fs::exists(path)) {
    fs::recursive_directory_iterator it(path);
    fs::recursive_directory_iterator endit;
    while (it != endit) {
      if (fs::is_regular_file(*it)) {
        fs::path rel_path = fs::relative(it->path(), path);
        filenames.push_back(rel_path.string());
        // filenames.push_back(it->path().string());
      }
      ++it;
    }
  } else {
    LOG(ERROR) << path << " is not existed!";
  }
}

bool CompareDirectories(const boost::filesystem::path& dir1,
                        const boost::filesystem::path& dir2) {
  namespace fs = boost::filesystem;
  std::vector<std::string> v1, v2;
  TraverseDirectory(dir1, v1);
  TraverseDirectory(dir2, v2);
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  bool result = std::equal(v1.begin(), v1.end(), v2.begin()) &&
                std::equal(v2.begin(), v2.end(), v1.begin());
  if (!result) {
    LOG(ERROR) << dir1 << " files: ";
    LOGContainer(v1);
    LOG(ERROR) << dir2 << " files: ";
    LOGContainer(v2);
  }
  return result;
}

bool CompareDirectoriesAndContents(const boost::filesystem::path& src,
                                   const boost::filesystem::path& dst) {
  namespace fs = boost::filesystem;
  std::vector<std::string> src_files;
  TraverseDirectory(src, src_files);
  for (const auto& path : src_files) {
    auto src_file = src / path;
    auto dst_file = dst / path;
    if (boost::filesystem::exists(dst_file)) {
      LOG(ERROR) << "dst file: " << dst_file << " is not existed!";
      return false;
    }
    bool result = CompareFiles(src_file, dst_file);
    if (!result) {
      LOG(ERROR) << src_file << " and " << dst_file << " content is not same!";
    }
  }
  return true;
}

void CreateDepthInternal(size_t depth, int num_dirs, int num_files,
                         const boost::filesystem::path& base_dir) {
  if (depth == 0) return;
  const std::string content = "Hello, World!";
  for (int dir_idx = 0; dir_idx < num_dirs; ++dir_idx) {
    std::string dir_name =
        "depth" + std::to_string(depth) + "-" + "dir" + std::to_string(dir_idx);
    boost::filesystem::path dir_path = base_dir / dir_name;
    if (boost::filesystem::create_directories(dir_path)) {
      for (int j = 0; j < num_files; ++j) {
        std::string file_name = dir_name + "-" + "file" + std::to_string(j);
        boost::filesystem::path file_path = dir_path / file_name;
        std::ofstream file(file_path.string());
        file << content;
        file.close();
      }
      CreateDepthInternal(depth - 1, num_files, num_dirs, dir_path);
    }
  }
}

std::string CreateDepthDirectoryRecursive(size_t depth, int num_dirs,
                                          int num_files,
                                          const std::string& dir_name) {
  auto dir = boost::filesystem::current_path() / dir_name;
  CreateDepthInternal(depth, num_dirs, num_files, dir);
  return dir.string();
}

void TestBoostFile() { CreateDepthDirectoryRecursive(2, 2, 2, "boost_test"); }


int main() {
  TestDefine();

  // TestCallOnce();

  // TestCall();

  TestPrintContainer();

  TestLogContainer();

  TestBoostFile();

  return 0;
}