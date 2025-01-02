#ifndef _GZN_PGK_HEIGHTMAPREADER_
#define _GZN_PGK_HEIGHTMAPREADER_

#include <array>
#include <cstdint>
#include <string_view>
#include <utility>
#include <vector>

struct HeightMap {
	inline static constexpr std::size_t side = 1201;
	inline static constexpr std::size_t size = side * side;

	std::array<uint16_t, size> data;
	int latitude;
	int longitude;
};

HeightMap* readFile(std::string_view file);

std::vector<HeightMap*> readData(std::string_view directory, std::pair<int, int> latitude, std::pair<int, int> longitude);

#endif
