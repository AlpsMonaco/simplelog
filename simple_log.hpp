#ifndef __SIMPLE_LOG_HPP__
#define __SIMPLE_LOG_HPP__

#ifdef SIMPLE_LOG_FILENAME
#define USER_DEFINE_SIMPLE_LOG_FILENAME SIMPLE_LOG_FILENAME
#endif

#ifdef SIMPLE_LOG_NO_PRINT
#define USE_DEFINE_SIMPLE_LOG_NO_PRINT
#else
#include <iostream>
#endif

#include <chrono>
#include <fstream>
#include <string>
#include <string_view>

#if defined(PLATFORM_POSIX) || defined(__linux__)
#include <mutex>
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <libloaderapi.h>
inline void localtime_r(const time_t* tp, tm* tm) { localtime_s(tm, tp); }
#endif

namespace simplelog {
class Log {
public:
  static void FormatLog(std::string_view log_type, std::string_view log_content);
  static void Info(std::string_view log_content);
  static void Error(std::string_view log_content);
  static void Debug(std::string_view log_content);
  static bool IsOpen();
  ~Log();

protected:
  Log();
  static Log& Instance();

  static constexpr std::string_view log_prefix_buffer_format = "[YYYY-mm-dd HH:MM:ss.";
  static constexpr size_t log_prefix_buffer_size = log_prefix_buffer_format.size() + 1;
  static constexpr std::string_view log_prefix_format = "[%Y-%m-%d %H:%M:%S.";

  std::ofstream ofs_;
  bool has_open_;
#if defined(PLATFORM_POSIX) || defined(__linux__)
  std::mutex mu_;
#endif
};

inline std::string GetLogName()
{
#ifdef USER_DEFINE_SIMPLE_LOG_FILENAME
  return USER_DEFINE_SIMPLE_LOG_FILENAME;
#endif
  std::string log_filename;
#if defined(PLATFORM_POSIX) || defined(__linux__)
  std::ifstream("/proc/self/comm") >> log_filename;
#elif defined(_WIN32)
  char buf[MAX_PATH + 1];
  GetModuleFileNameA(nullptr, buf, MAX_PATH);
  log_filename = buf;
#else
  static_assert(false, "unrecognized platform");
#endif
  size_t pos = log_filename.find_last_of("\\/");
  if (pos != std::string::npos)
    log_filename = log_filename.substr(pos + 1);
  pos = log_filename.rfind(".");
  if (pos != std::string::npos)
    log_filename = log_filename.substr(0, pos);
  return log_filename + ".log";
}

inline void Log::FormatLog(std::string_view log_type, std::string_view log_content)
{
  auto now = std::chrono::system_clock::now().time_since_epoch();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
  auto s = std::chrono::duration_cast<std::chrono::seconds>(now).count();
  tm tm;
  localtime_r(&s, &tm);
  char time_prefix_buffer[log_prefix_buffer_size];
  strftime(time_prefix_buffer, log_prefix_buffer_size, log_prefix_format.data(), &tm);
  std::string log_buffer(time_prefix_buffer);
  ms -= s * 1000;
  if (ms < 100)
    log_buffer += '0';
  if (ms < 10)
    log_buffer += '0';
  log_buffer += std::to_string(ms);
  log_buffer += "] [";
  log_buffer += log_type;
  log_buffer += "] ";
  log_buffer += log_content;
  log_buffer += '\n';
#ifndef USE_DEFINE_SIMPLE_LOG_NO_PRINT
  std::cout << log_buffer;
#endif
  if (IsOpen()) {
#if defined(PLATFORM_POSIX) || defined(__linux__)
    std::lock_guard<std::mutex> lg(Instance().mu_);
#endif
    Instance().ofs_ << log_buffer;
  }
}

inline void Log::Info(std::string_view log_content)
{
  FormatLog("INFO", log_content);
}

inline void Log::Error(std::string_view log_content)
{
  FormatLog("ERROR", log_content);
}

inline void Log::Debug(std::string_view log_content)
{
#ifndef NDEBUG
  FormatLog("DEBUG", log_content);
#endif
}

inline bool Log::IsOpen()
{
  return Instance().has_open_;
}

inline Log::Log()
    : ofs_(GetLogName(), std::ios_base::app)
    , has_open_(ofs_.is_open())
{
}

inline Log& Log::Instance()
{
  static Log log;
  return log;
}

inline Log::~Log()
{
}

}

#endif