#include "mainScene.hpp"
#include "core/appdata.hpp"
#include <filesystem>
#include <iostream>
#include <regex>

MainScene::MainScene() {
	std::regex filePattern(R"(([NS])(\d{2})([WE])(\d{3})\.hgt)");
	std::smatch match;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(HeightMap::readDirectory)) {
            if (!entry.is_regular_file()) continue;
			std::string fileName = entry.path().filename().string();
			if (!std::regex_match(fileName, match, filePattern)) continue;

			const int latSign = match[1].str()[0] == 'N' ? 1 : -1;
			const int lat = std::stoi(match[2].str()) * latSign;
			const int lonSign = match[3].str()[0] == 'E' ? 1 : -1;
			const int lon = std::stoi(match[4].str()) * lonSign;

			chunks.emplace_back(lat, lon);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: unable to access directory: " << e.what() << std::endl;
    }
}

void MainScene::update() {

}

void MainScene::render() {
	const std::size_t lod = 0;
	AppData::Data().shaders.map2D.bind();
	AppData::Data().shaders.map2D.setUniform("cameraPos", 1.0f, 0.5f);
	AppData::Data().shaders.map2D.setUniform("scale", 1.f, 1.f);
	AppData::Data().shaders.map2D.setUniform("side", 1201u);
	// chunks[0].render(lod);
	// chunks[1].render(lod);
}
