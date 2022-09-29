# simplelog
`simplelog` a header only log library and aims to provide out-of-the-box log feature and simple API.  
It is super tiny and the lines of the code is about 150 but still thread safe.  
You just have to log in most scenarios,so why bother?

# Application Scenarios
small projects or non-performance-sensitive projects,just include it and log,no further configuration need to set.  

# Quick Start
```c++
#include <simple_log.hpp>
namespace sl = simplelog;

int main(int argc, char** argv)
{
  sl::Log::Info("info message");
  sl::Log::Error("error message");
  sl::Log::Debug("debug message");
}

```
By default,the log file name is your executable name.Assumes that your executable name is `calculate`,  
then a file `calculate.log` will be created and logged to whenever any `simple::Log` function is called.  
You could also use macro `SIMPLE_LOG_FILENAME` to custom your own log name instead of executable name.  

```c++
#define SIMPLE_LOG_FILENAME "trivial.log"
#include <simple_log.hpp>

...
```

# Design 

The performance of a widely used C++ log library is normally decent,but it is also huge and perhaps hard to configure.  
The library provides simple api and header file that make it easy for anyone to read or DIY.  


## Thread Safety
On `Windows` an one-time output stream operation is thread safe.  
```c++
std::string s;
std::ostream os;
// it is thread safe on Windows.
os << s;

// not thread safe because it is a two-time
// output stream operation.
os << s << std::endl
```
So on windows,the library use system io to keep log content in sync.  

But on `Linux`,an one-time output stream operation could be thread safe or not.  
It is depends on `Linux` release version or kernel version.  
So there's a `mutex` to keep log content in sync on `Linux` version.  


## Performance
This library is not designed for high performance scenarios.  
But on a small project or a non-performance-sensitive project,this library is easy to use and fast to begin coding.  
It is also strange when the size of a log library is larger than your own project.  
You could create a thread pool and then call `simplelog::Log` function to improve speed because  
it is thread safe.  
