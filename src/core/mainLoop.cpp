#include "mainLoop.hpp"

#include "window.hpp"
#include "input.hpp"
#include "appdata.hpp"

#include <chrono>
#include <iostream>

void MainLoop::update() {
	Input::update();
	AppData::Data().mainScene.update();
	HeightMap::GenerateRequestedBuffers();
}

void MainLoop::render() {
	AppData::Data().window.endFrame();
	AppData::Data().mainScene.render();
}

void MainLoop::run() {
	std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::duration<double> timeBetweenFrames{};
	const auto timePerUpdate = AppData::Data().timePerUpdate;
	double lag = 0.0;
	unsigned int fpsCount = 0;

	constexpr double minFPS = 10;
	constexpr double maxFPS = 60;
	constexpr double fpsInterval = 0.5;
	constexpr int fpsSkipDisplay = 20;
	double fpsTimer = 0.0;
	int fpsShowCount = 0;

	while (!AppData::Data().window.shouldClose()) {
		now = std::chrono::steady_clock::now();
		timeBetweenFrames = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime);
		lastFrameTime = std::chrono::steady_clock::now();

		lag += timeBetweenFrames.count();
		fpsTimer += timeBetweenFrames.count();

		while(lag >= timePerUpdate ) {
			lag -= timePerUpdate;
			update();
		}

		render();

		fpsCount++;
		if (fpsTimer >= fpsInterval) {
			unsigned int fps = static_cast<unsigned int>(static_cast<double>(fpsCount) / fpsInterval);
			unsigned int tps = static_cast<unsigned int>(static_cast<double>(AppData::trianglesDrawn) / fpsInterval);
			fpsTimer = 0.0;
			fpsCount = 0;
			AppData::trianglesDrawn = 0;

			if(AppData::lod == 0) {
				if(fps < minFPS && HeightMap::lod < HeightMap::LODS - 1)
					HeightMap::lod++;
				if(fps >= maxFPS && HeightMap::lod > 0)
					HeightMap::lod--;
			} else {
				HeightMap::lod = AppData::lod - 1;
			}

			fpsShowCount++;
			if(fpsShowCount == fpsSkipDisplay) continue;
			fpsShowCount = 0;
			std::cout << "FPS: " << fps << ", TPS: " << tps << ", lod = " << HeightMap::lod + 1 << "\n";
		}
	}
}
