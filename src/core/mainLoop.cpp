#include "mainLoop.hpp"

#include "window.hpp"
#include "input.hpp"
#include "appdata.hpp"

#include <chrono>

void MainLoop::update() {
	Input::update();
	AppData::Data().mainScene.update();
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

	while (!AppData::Data().window.shouldClose()) {
		now = std::chrono::steady_clock::now();
		timeBetweenFrames = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime);
		lastFrameTime = std::chrono::steady_clock::now();

		lag += timeBetweenFrames.count();

		while(lag >= timePerUpdate ) {
			lag -= timePerUpdate;
			update();
		}

		render();
	}
}
