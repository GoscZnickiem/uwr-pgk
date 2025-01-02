#ifndef _GZN_PGK_HEIGHTMAPREADER_
#define _GZN_PGK_HEIGHTMAPREADER_

#include "heightMap.hpp"

#include <string_view>
#include <utility>
#include <vector>

HeightMap* readFile(std::string_view file);

std::vector<HeightMap*> readData(std::string_view directory, std::pair<int, int> latitude, std::pair<int, int> longitude);

#endif
