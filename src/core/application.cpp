#include "application.hpp"

#include "window.hpp"
#include "input.hpp"
#include "appdata.hpp"

#include <chrono>
#include <iostream>

Application::Application() {
	AppData::Init();
}

Application::~Application() {
	AppData::Terminate();
}

void update() {
	Input::update();
}

void render() {
	AppData::Data().window.endFrame();
}

void run() {
	std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::duration<double> timeBetweenFrames{};
	const auto timePerUpdate = AppData::Data().timePerUpdate;
	double lag = 0.0;

	double timer = 0.0;

	while (!AppData::Data().window.shouldClose()) {
		now = std::chrono::steady_clock::now();
		timeBetweenFrames = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime);
		lastFrameTime = std::chrono::steady_clock::now();

		lag += timeBetweenFrames.count();
		timer += timeBetweenFrames.count();

		while(lag >= timePerUpdate ) {
			lag -= timePerUpdate;
			update();
		}

		render();
	}
}

void atResize(int width, int height) {
	std::cout << "Resize: " << width << ", " << height << "\n";
}
