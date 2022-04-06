#include "Tests/RunTests.hpp"
#include "Tests/ColladaParserTests.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <filesystem>

static std::map<std::string, int (*)(TestType type, std::string path)> tests =
{
  {"testParser", testParser}
};

TestType getTypeFromStr(const std::string& typeStr)
{
  if (typeStr == "Full")
    return TestType::Full;
  else if (typeStr == "SingleFile")
    return TestType::SingleFile;
  return TestType::Invalid;
}

void removeComa(std::string& str) {
  if (str.back() != ',')
    throw std::runtime_error("Invalid test '" + str + "' declaration: missing ','");
  //  Simulate removal of the last char ','
  str[str.size() - 1] = '\0';
}

void runTests(const std::string& testFile)
{
  std::ifstream ifs;

  try
  {
    ifs.open(testFile);
  }
  catch (std::ifstream::failure& e)
  {
    std::cerr << "Could not open tests list file '" + testFile + "'" << std::endl;
    return ;
  }
  std::vector<Test> testsList;
  while (ifs)
  {
    std::string line;
    getline(ifs, line);
    if (line.empty())
      continue;
    std::stringstream str(line);
    std::string testName;
    str >> testName;
    if (!str)
      throw std::runtime_error("Invalid test '" + testName + "': missing type");
    std::string typeStr;
    str >> typeStr;
    TestType type = getTypeFromStr(typeStr);
    if (type == TestType::Invalid)
      throw std::runtime_error("Invalid test '" + testName + "' type: " + typeStr);
    std::string path;
    if (str)
      str >> path;
    testsList.push_back(Test(testName, type, typeStr, path));
  }
  for (const auto& test: testsList)
  {
    std::cout << "Running test '" << test.name << "'...";
    std::cout << " in " << test.typeStr;
    if (test.path.empty())
      std::cout << "( path: '" << test.path << "' )";
    std::cout << " mode " << std::endl;
    std::string pathStr = LOGS_DIRECTORY;
    pathStr += "/" + test.name;
    std::filesystem::path path(pathStr);
    if (!std::filesystem::exists(path))
    {
      std::filesystem::create_directories(path);
    }
    int res = RUNTEST(tests[test.name], test.type, test.path);
    if (res == 0)
      std::cout << "--VALID--" << std::endl;
    else
    {
      std::cout << "--INVALID--" << std::endl;
    }
  }
}
