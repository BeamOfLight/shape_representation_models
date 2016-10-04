/**
  https://github.com/BeamOfLight/shape_representation_models.git
  filesystem.cpp

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#include <dpcore/filesystem.h>

std::string DpCore::Filesystem::getRootProjectDirectory()
{
  std::string addition_path;
#ifdef __linux__
  addition_path = "/../";
#else
  addition_path = "\\..\\..\\..\\";
#endif
  return DpCore::Filesystem::getExePath() + addition_path;
}

std::string DpCore::Filesystem::getExePath()
{
  return std::string(getcwd(NULL,0));
}

bool DpCore::Filesystem::isFileExist(const std::string &filename)
{
  struct stat info = {0};

  return stat(filename.c_str(), &info) == 0;
}

bool DpCore::Filesystem::isDirExist(const std::string &path)
{
  struct stat info = {0};
  if (stat(path.c_str(), &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}

std::string DpCore::Filesystem::canonizePath(const std::string &path)
{
  std::string canonizedPath = path;
  for (size_t i = 0; i < canonizedPath.length(); i++)
    if (canonizedPath[i] == '\\')
      canonizedPath[i] = '/';

  return canonizedPath;
}

std::string DpCore::Filesystem::getCrossPlatformFileName(const std::string &path)
{
#ifdef __linux__
  std::string result = canonizePath(path);
#else
  std::string result = canonizePath(path);
  for (size_t i = 0; i < result.length(); i++) {
    if (result[i] == '/') {
        result[i] = '\\';
    }
  }
#endif

  return  DpCore::Filesystem::getRootProjectDirectory() + result;
}

std::string DpCore::Filesystem::fileName(const std::string &path)
{
  size_t pos = canonizePath(path).rfind("/");

  return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

std::string DpCore::Filesystem::fileBaseName(const std::string &path)
{
  size_t pos = canonizePath(path).rfind(".");

  return fileName(pos == std::string::npos ? path : path.substr(0, pos));
}

std::string DpCore::Filesystem::fileExtension(const std::string &path)
{
  size_t pos = canonizePath(path).rfind(".");

  return (pos == std::string::npos) ? std::string() : path.substr(pos + 1);
}

