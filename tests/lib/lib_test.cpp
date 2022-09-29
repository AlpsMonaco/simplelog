#include <simple_log.hpp>

namespace sl = simplelog;

void CallLibTest()
{
  sl::Log::Info("from lib_test");
  sl::Log::Error("from lib_test");
  sl::Log::Debug("from lib_test");
}