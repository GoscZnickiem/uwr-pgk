#include "mainScene.hpp"
#include "core/appdata.hpp"
#include "core/input.hpp"
#include <cmath>
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

			const char latSign = match[1].str()[0];
			const int lat = std::stoi(match[2].str());
			const char lonSign = match[3].str()[0];
			const int lon = std::stoi(match[4].str());

			const int y = latSign == 'N' ? lat + 90 : 90 - lat;
			const int x = lonSign == 'E' ? lon + 180 : 180 - lon;

			chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].state = HeightMap::State::UNLOADED;
			std::cout << x << " " << y << " (" << fileName << ")\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: unable to access directory: " << e.what() << std::endl;
    }

	cameraPos = {14, 50};
	scale = {0.2f, 0.2f};
}

void MainScene::update() {
	areaXmin = static_cast<int>( std::floor(cameraPos.x - 1 / scale.x) ) + 180 - 1;
	if(areaXmin < 0) areaXmin += 360;
	areaXmax = static_cast<int>( std::ceil(cameraPos.x + 1 / scale.x) ) + 180 + 1;
	if(areaXmax >= 360) areaXmax -= 360;

	areaYmin = static_cast<int>( std::floor(cameraPos.y - 1 / scale.y) ) + 90 - 1;
	if(areaYmin < 0) areaYmin += 180;
	areaYmax = static_cast<int>( std::ceil(cameraPos.y + 1 / scale.y) ) + 90 + 1;
	if(areaYmax >= 180) areaYmax -= 180;

	// load visible chunks:
	for(int x = areaXmin; x != areaXmax; x++) {
		if(x == 360) x = 0;
		for(int y = areaYmin; y != areaYmax; y++) {
			if(y == 180) y = 0;

			auto& c = chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
			c.load(y - 90, x - 180);
		}
	}

	// unload invisible ones:
	int borderLeft = areaXmin == 0 ? 359 : areaXmin - 1;
	int borderRight = areaXmax == 359 ? 0 : areaXmax + 1;
	int borderTop = areaYmin == 0 ? 180 : areaYmin - 1;
	int borderBottom = areaYmax == 179 ? 0 : areaYmax + 1;
	for(int x = borderLeft; x != borderRight; x++) {
		if(x == 360) x = 0;
		const int y = borderTop;
		chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].unload(y - 90, x - 180);
	}
	for(int x = borderLeft; x != borderRight; x++) {
		if(x == 360) x = 0;
		const int y = borderBottom;
		chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].unload(y - 90, x - 180);
	}
	for(int y = borderBottom; y != borderTop; y++) {
		if(y == 360) y = 0;
		const int x = borderLeft;
		chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].unload(y - 90, x - 180);
	}
	for(int y = borderBottom; y != borderTop; y++) {
		if(y == 360) y = 0;
		const int x = borderRight;
		chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].unload(y - 90, x - 180);
	}

	// camera movement
	if(Input::isKeyPressed("W")) cameraPos.y += AppData::deltaT;
	if(Input::isKeyPressed("S")) cameraPos.y -= AppData::deltaT;
	if(Input::isKeyPressed("A")) cameraPos.x -= AppData::deltaT;
	if(Input::isKeyPressed("D")) cameraPos.x += AppData::deltaT;
}

void MainScene::render() {
	const std::size_t lod = 1;
	AppData::Data().shaders.map2D.bind();
	AppData::Data().shaders.map2D.setUniform("cameraPos", cameraPos);
	AppData::Data().shaders.map2D.setUniform("scale", scale);

	for(int x = areaXmin; x != areaXmax; x++) {
		if(x == 360) x = 0;
		for(int y = areaYmin; y != areaYmax; y++) {
			if(y == 180) y = 0;

			auto& c = chunks[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
			if(c.state != HeightMap::State::LOADED) continue;
			AppData::Data().shaders.map2D.setUniform("position", x-180, y-90);
			c.render(lod);
		}
	}
}
