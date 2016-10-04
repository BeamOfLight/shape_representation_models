/**
  https://github.com/BeamOfLight/shape_representation_models.git
  filesystem.h

  @author Denis Borisoglebskiy
  @version 1.0 2016-10-04
*/

#pragma once
#ifndef DPCORE_FILESYSTEM_H
#define DPCORE_FILESYSTEM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>

#ifdef __linux__
  #include <unistd.h>
#else
  #include <direct.h>
    #define getcwd _getcwd // stupid MSFT "deprecation" warning
#endif

namespace DpCore
{
  class Filesystem
  {
    public:
      static std::string getRootProjectDirectory();				  // Путь к корневой директории проекта
      static std::string canonizePath(const std::string &path);   // Возвращает путь в канонизированном виде (прямой слеш)
      static std::string fileName(const std::string &path);       // Возвращает имя файла (включая расширение)
      static std::string getCrossPlatformFileName(const std::string &path); // Возвращает имя файла (включая расширение) от корня проекта
      static std::string fileBaseName(const std::string &path);   // Возвращает базовое имя файла (без расширения)
      static std::string fileExtension(const std::string &path);  // Возвращает расширение файла
      static bool isFileExist(const std::string &path);           // Возвращает true, если файл существует
      static bool isDirExist(const std::string &path);            // Возвращает true, если директория существует
      static std::string getExePath();							  // Возвращает текущую директорию исполняемого файла
  };
}

#endif
