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
        for (const auto& entry : std::filesystem::recursive_directory_iterator(AppData::readDirectory)) {
            if (!entry.is_regular_file()) continue;
			std::string fileName = entry.path().filename().string();
			if (!std::regex_match(fileName, match, filePattern)) continue;
			std::string fullName = entry.path().string();

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

			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			c.state = HeightMap::State::UNLOADED;
			c.sourceFile = fullName;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: unable to access directory: " << e.what() << std::endl;
    }

	cameraPos = {13, 50};
	scale = 0.5f;

	camera.up = {0, 1, 0};
	camera.direction = {-1, 0, 0};
	cameraHeight = 50.0f;
}

void MainScene::update() {
	const float speed = AppData::deltaT / scale * 1.f;
	constexpr float earthRadius = 6378.f;

	if(view2D) {
		areaXmin = static_cast<int>( std::floor(cameraPos.x - 1 / scale / aspectRatio.x) ) + 180 - 1;
		if(areaXmin < 0) areaXmin += 360;
		areaXmax = static_cast<int>( std::ceil(cameraPos.x + 1 / scale / aspectRatio.x) ) + 180 + 1;
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
				c.load();
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
			chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload();
		}
		for(int x = borderLeft; x != borderRight; x++) {
			if(x == 360) { x = -1; continue; }
			const int y = borderBottom;
			chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload();
		}
		for(int y = borderBottom; y != borderTop; y++) {
			const int x = borderLeft;
			chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload();
		}
		for(int y = borderBottom; y != borderTop; y++) {
			const int x = borderRight;
			chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)].unload();
		}

		// camera movement
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
			if(scale < 0.01f) scale = 0.01f;
		}
	} else {
		// camera movement
		// if(Input::isKeyPressed("W")) { cameraPos.y += speed; if(cameraPos.y > 90) cameraPos.y = 90; }
		// if(Input::isKeyPressed("S")) { cameraPos.y -= speed; if(cameraPos.y < -90) cameraPos.y = -90; }
		// if(Input::isKeyPressed("A")) { cameraPos.x -= speed; if(cameraPos.x < -180) cameraPos.x += 360; }
		// if(Input::isKeyPressed("D")) { cameraPos.x += speed; if(cameraPos.x >= 180) cameraPos.x -= 360; }

		if(Input::isKeyClicked("+") || Input::getScroll() > 0) {
			cameraHeight += 1.f;
			if(cameraHeight > 2 * earthRadius) cameraHeight = 2 * earthRadius;
		}
		if(Input::isKeyClicked("-") || Input::getScroll() < 0) {
			cameraHeight -= 1.f;
			if(cameraHeight < 0) cameraHeight = 0;
		}

		const float sinx = std::sin(3.1415f/180 * cameraPos.x);
		const float cosx = std::cos(3.1415f/180 * cameraPos.x);
		const float siny = std::sin(3.1415f/180 * cameraPos.y);
		const float cosy = std::cos(3.1415f/180 * cameraPos.y);
		camera.position = (earthRadius + cameraHeight) * glm::vec3{cosy * cosx, siny, -cosy * sinx};

		camera.update();
	}

	// LOD control (kontrola nad lodem)
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

	if(Input::isKeyClicked("TAB")) {
		Input::setMousePosLock(view2D);
		view2D = !view2D;
	}
}

void MainScene::render() {
	const Shader* shader;
	if(view2D) {
		shader = &AppData::Data().shaders.map2D;
		shader->bind();
		shader->setUniform("cameraPos", cameraPos);
		shader->setUniform("scale", aspectRatio * scale);
	} else {
		shader = &AppData::Data().shaders.map3D;
		shader->bind();
		shader->setUniform("projection", camera.getProjectionMatrix());
		shader->setUniform("view", camera.getViewMatrix());
	}

	for(int x = areaXmin; x != areaXmax; x++) {
		if(x == 360) { x = -1; continue; }
		for(int y = areaYmin; y != areaYmax; y++) {

			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			if(c.state != HeightMap::State::LOADED) continue;
			shader->setUniform("position", x-180, y-90);
			c.render();
		}
	}
}

void MainScene::atResize(int width, int height) {
	aspectRatio = {static_cast<float>(height)/static_cast<float>(width), 1};
	camera.aspectRatio = aspectRatio.x;
}
