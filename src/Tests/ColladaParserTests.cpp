#include "Tests/ColladaParserTests.hpp"
#include "Object/AssetManager.class.hpp"
#include "Object/GLObject.class.hpp"
#include "ColladaParser/ColladaException.class.hpp"

#include <filesystem>
#include <csignal>

using namespace notrealengine;

void catchSegfault(int sigNum)
{
  std::cout << "Signal " << sigNum << " received" << std::endl;
}

static int compareFile(const std::string& path)
{
#ifdef USING_EXTERNAL_LIBS
#define WAS_USING
#undef USING_EXTERNAL_LIBS
#endif

  std::string outputFile = LOGS_DIRECTORY;
  outputFile += "/compareFile/" + path.substr(path.find_last_of('/') + 1);
  try
  {
    signal(SIGSEGV, catchSegfault);
    signal(SIGABRT, catchSegfault);
    GLObject obj1(path);
  }
  catch (ColladaException& e)
  {
    std::cerr << "Could not load '" << path << "' with custom parser" << std::endl;
    return -1;
  }

#define USING_EXTERNAL_LIBS
  GLObject obj2(path);
  return 0;
#ifndef WAS_USING
#undef USING_EXTERNAL_LIBS
#endif
}

int testParser(TestType type, std::string path)
{
  if (type == TestType::SingleFile)
  {
    compareFile(path);
  }
  else
  {
    std::string dir = "./resources/objects";
    for (const auto& file: std::filesystem::directory_iterator(dir))
    {
      std::filesystem::path filePath = file.path();
      if (filePath.extension() == ".dae")
        compareFile(filePath.string());
    }
  }
  return 0;
}
