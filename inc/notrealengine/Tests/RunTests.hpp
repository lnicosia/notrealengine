#ifndef _RUN_TESTS_H_
#define _RUN_TESTS_H_

#include <string>

#define TESTS_DIRECTORY "./test"
#define LOGS_DIRECTORY TESTS_DIRECTORY "/logs"

enum class TestType
{
  Invalid,
  Full,
  SingleFile,
};

struct  Test
{
  std::string name;
  TestType    type;
  std::string typeStr;
  std::string path;

  Test(const std::string& name, const TestType type, const std::string& typeStr,
    const std::string& path):
    name(name), type(type), typeStr(typeStr), path(path) {}
};

template < typename ... Args>
int runTest(int (*func)(Args ...), Args ... args)
{
  return func(std::forward<Args>(args)...);
}

#define RUNTEST(func_name, ...) \
  runTest(func_name __VA_OPT__(,) __VA_ARGS__);

void
    runTests(const std::string& testFile);

#endif
