#ifndef CHESS_PATHS_H
#define CHESS_PATHS_H

#include <string>
#include <filesystem>
#include <iostream>

namespace paths {
  std::string getProjectRootPath();
  std::string getImagesPath();
  std::string getSoundsPath();
  std::string getFontsPath();
}

#endif//CHESS_PATHS_H
