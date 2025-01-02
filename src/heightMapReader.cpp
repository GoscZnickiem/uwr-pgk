#include "heightMapReader.hpp"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

inline static constexpr uint16_t swapBytes(uint16_t v) {
	return (v >> 8) | (v << 8);
}

std::vector<std::unique_ptr<HeightMap>> heightMaps;

HeightMap* readFile(std::string_view fileName) {
	std::ifstream file(fileName.data(), std::ios::binary);
	if(!file) return nullptr;

	std::unique_ptr<HeightMap> map = std::make_unique<HeightMap>();
	file.read(reinterpret_cast<char*>(map->data.data()), HeightMap::size * 2);
	if (!file) {
		std::cerr << "Error: Failed to read the file \"" << fileName << "\". Bytes read: " << file.gcount() << "\n";
		exit(3);
	}
	file.close();

	for (auto& v : map->data)
	v = swapBytes(v);

	auto* ptr = map.get();
	heightMaps.push_back(std::move(map));
	return ptr;
}

std::vector<HeightMap*> readData(std::string_view directory, std::pair<int, int> latitude, std::pair<int, int> longitude) {
	std::vector<HeightMap*> maps;

	for(int lat = latitude.first; lat <= latitude.second; lat++) {
		if(lat == 0) continue;
		for(int lon = longitude.first; lon <= longitude.second; lon++) {
			if(lon == 0) continue;

			const char latSign = lat < 0 ? 'S' : 'N';
			const char lonSign = lon < 0 ? 'W' : 'E';

			std::stringstream ss;
			ss	<< directory 
				<< latSign 
				<< std::setw(2) << std::setfill('0') << std::abs(lat)
				<< lonSign
				<< std::setw(3) << std::setfill('0') << std::abs(lon)
				<< ".hgt";
			auto* map = readFile(ss.str());
			if(map != nullptr) {
				map->latitude = lat;
				map->longitude = lon;
				maps.push_back(map);
			} 
		}
	}

	if(maps.empty()) {
		std::cerr << "Error: No files have been read.\n";
		exit(4);
	}

	return maps;
}
