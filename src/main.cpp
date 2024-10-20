#include "window.hpp"
#include "appdata.hpp"
#include "input.hpp"
#include "obstacle.hpp"
#include "player.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <random>

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
	std::vector<Obstacle> obstacles;
	Player player{-0.5f, -0.5f};

	Application(long unsigned int seed = static_cast<long unsigned int>(std::time(nullptr)), int size = 10) {
		obstacles.reserve(static_cast<size_t>(size) * static_cast<size_t>(size));
		std::mt19937 gen(seed);

		const float boardSize = 1.8f;
		const float distance = boardSize / static_cast<float>(size);

		std::uniform_real_distribution<float> angle(-3.14f, 3.14f);
		std::uniform_real_distribution<float> scale(0.5f, 1.0f);
		const float stdScale = 0.03f;

		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				obstacles.emplace_back(
					static_cast<float>(i) * distance - boardSize / 2,
					static_cast<float>(j) * distance - boardSize / 2, 
					angle(gen), scale(gen) * stdScale, scale(gen) * stdScale * 3);
			}
		}
	}

	void update() {
		player.update();
		if(Input::isKeyPressed("ESCAPE"))
			window.close();
	}

	void render() {
		for(auto& o : obstacles) o.render();
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

void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length, [[maybe_unused]] const GLchar* message, [[maybe_unused]] const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		std::cerr << "GL Error: " << message << "\n";
}

int main() {
	initializeGLFW();
	Application app(4);

	glDebugMessageCallback(MessageCallback, nullptr);

	app.run();
}

