#include "window.hpp"
#include "appdata.hpp"
#include "input.hpp"
#include "obstacle.hpp"
#include "player.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <chrono>

void initializeGLFW() {
	if (glfwInit() == 0) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}
	std::atexit([](){
		glfwTerminate();
	});
}

struct Application {
	Window window{};
	Obstacle obs{0, 0, 0, 0.1f, 0.1f};
	Player player{-0.5f, -0.5f};

	void update() {
		if(Input::isKeyPressed("ESCAPE"))
			window.close();
		player.update();
	}

	void render() {
		obs.render();
		player.render();

		window.endFrame();
	}

	void run() {
		std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeBetweenFrames{};
		double lag = 0.0;

		const double fpsInterval = 1.;
		double fpsTimer = 0.0;
		uint fpsCount = 0;

		const auto timePerUpdate = AppData::data().timePerUpdate;

		while (!window.shouldClose()) {
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
				fpsTimer = 0.;
				std::cout << "FPS: " << static_cast<uint>(static_cast<double>(fpsCount) / fpsInterval) << "\n";
				fpsCount = 0;
			}
		}
	}
};

int main() {
	initializeGLFW();
	Application app;
	app.run();
}

