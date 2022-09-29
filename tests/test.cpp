#include <lib_test.h>
#include <simple_log.hpp>
#include <thread>
#include <vector>

namespace sl = simplelog;

int main(int argc, char** argv)
{
  std::cout << simplelog::GetLogName() << std::endl;
  std::vector<std::thread> thread_list;
  for (size_t i = 0; i < 10; i++) {
    thread_list.emplace_back(
        []() -> void {
          for (size_t i = 0; i < 30; i++) {
            sl::Log::Info("info");
            sl::Log::Error("error");
            sl::Log::Debug("debug");
            CallLibTest();
          }
        });
  }
  for (auto& t : thread_list)
    t.join();
}