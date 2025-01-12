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

			if(y < AppData::latBounds.first || y > AppData::latBounds.second) continue;
			if(AppData::lonBounds.first <= AppData::lonBounds.second) {
				if(x < AppData::lonBounds.first || x > AppData::lonBounds.second) continue;
			} else {
				if(x < AppData::lonBounds.first && x > AppData::lonBounds.second) continue;
			}

			chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].state = HeightMap::State::UNLOADED;
			std::cout << x << " " << y << " (" << fileName << ")\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: unable to access directory: " << e.what() << std::endl;
    }

	for(std::size_t i = 0; i < chunks.size(); i++) {
		std::cout << "x = " << i << ": ";
		for(auto& c : chunks[i]) {
			std::cout << (c.state == HeightMap::State::UNAVAIBLE ? " " : "#");
		}
		std::cout << "\n";
	}

	cameraPos = {12, 48};
	scale = 0.2f;
}

void MainScene::update() {
	areaXmin = static_cast<int>( std::floor(cameraPos.x - 1 / scale) ) + 180 - 1;
	if(areaXmin < 0) areaXmin += 360;
	areaXmax = static_cast<int>( std::ceil(cameraPos.x + 1 / scale) ) + 180 + 1;
	if(areaXmax >= 360) areaXmax -= 360;

	areaYmin = static_cast<int>( std::floor(cameraPos.y - 1 / scale) ) + 90 - 1;
	if(areaYmin < 0) areaYmin = 0;
	if(areaYmin >= 180) areaYmin = 179;
	areaYmax = static_cast<int>( std::ceil(cameraPos.y + 1 / scale) ) + 90 + 1;
	if(areaYmax < 0) areaYmax = 0;
	if(areaYmax >= 180) areaYmax = 179;

	// load visible chunks:
	for(int x = areaXmin; x != areaXmax; x++) {
		if(x == 360) { x = -1; continue; }
		for(int y = areaYmin; y != areaYmax; y++) {
			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			c.load(y - 90, x - 180);
		}
	}

	// unload invisible ones:
	int borderLeft = areaXmin == 0 ? 359 : areaXmin - 1;
	int borderRight = areaXmax == 359 ? 0 : areaXmax + 1;
	int borderBottom = areaYmin == 0 ? 0 : areaYmin - 1;
	int borderTop = areaYmax == 179 ? 179 : areaYmax + 1;
	for(int x = borderLeft; x != borderRight; x++) {
		if(x == 360) { x = -1; continue; }
		const int y = borderTop;
		chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload(y - 90, x - 180);
	}
	for(int x = borderLeft; x != borderRight; x++) {
		if(x == 360) { x = -1; continue; }
		const int y = borderBottom;
		chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload(y - 90, x - 180);
	}
	for(int y = borderBottom; y != borderTop; y++) {
		const int x = borderLeft;
		chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload(y - 90, x - 180);
	}
	for(int y = borderBottom; y != borderTop; y++) {
		const int x = borderRight;
		chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload(y - 90, x - 180);
	}

	// camera movement
	const float speed = AppData::deltaT / scale * 1.f;
	if(Input::isKeyPressed("W")) cameraPos.y += speed;
	if(Input::isKeyPressed("S")) cameraPos.y -= speed;
	if(Input::isKeyPressed("A")) { cameraPos.x -= speed; if(cameraPos.x < -180) cameraPos.x += 360; }
	if(Input::isKeyPressed("D")) { cameraPos.x += speed; if(cameraPos.x >= 180) cameraPos.x -= 360; }

	if(Input::isKeyClicked("+") || Input::getScroll() > 0) {
		scale *= 1.25f;
		if(scale > 10.f) scale = 10.f;
	}
	if(Input::isKeyClicked("-") || Input::getScroll() < 0) {
		scale *= 0.8f;
		if(scale < 0.05f) scale = 0.05f;
	}

	// LOD control
	if(Input::isKeyPressed("0")) AppData::lod = 0;
	if(Input::isKeyPressed("1")) AppData::lod = 1;
	if(Input::isKeyPressed("2")) AppData::lod = 2;
	if(Input::isKeyPressed("3")) AppData::lod = 3;
	if(Input::isKeyPressed("4")) AppData::lod = 4;
	if(Input::isKeyPressed("5")) AppData::lod = 5;
	if(Input::isKeyPressed("6")) AppData::lod = 6;
	if(Input::isKeyPressed("7")) AppData::lod = 7;
	if(Input::isKeyPressed("8")) AppData::lod = 8;
	if(Input::isKeyPressed("9")) AppData::lod = 9;
}

void MainScene::render() {
	AppData::Data().shaders.map2D.bind();
	AppData::Data().shaders.map2D.setUniform("cameraPos", cameraPos);
	AppData::Data().shaders.map2D.setUniform("scale", aspectRatio * scale);

	for(int x = areaXmin; x != areaXmax; x++) {
		if(x == 360) { x = -1; continue; }
		for(int y = areaYmin; y != areaYmax; y++) {

			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			if(c.state != HeightMap::State::LOADED) continue;
			AppData::Data().shaders.map2D.setUniform("position", x-180, y-90);
			c.render();
		}
	}
}

void MainScene::atResize(int width, int height) {
	aspectRatio = {static_cast<float>(width)/static_cast<float>(height), 1};
}
