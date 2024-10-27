#include "core/window.hpp"
#include "core/input.hpp"
#include "appdata.hpp"
#include "gameobjects/player.hpp"
#include "gameobjects/finish.hpp"
#include "gameobjects/obstacleCollection.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>
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

float lerp(float a, float b, float t) {
	return std::lerp(a, b, std::clamp(t, 0.f, 1.f));
}

struct Application {
	Window window{};

	const float boardSize = 1.8f;
	const float gridSize;

	ObstacleCollection obstacles;
	Player player;
	Finish finish;

	bool endAnimation = false;
	float animTime = 0.0;
	static constexpr float animDuration = 3.0;

	Application(long unsigned int seed = static_cast<long unsigned int>(std::time(nullptr)), int size = 3)
		: gridSize(boardSize / static_cast<float>(size)), 
		  obstacles(seed, size, boardSize, gridSize),
		  player(-boardSize / 2 + gridSize / 2, -boardSize / 2 + gridSize / 2, -0.78f, gridSize * 0.1f, gridSize * 0.3f),
		  finish( boardSize / 2 - gridSize / 2,  boardSize / 2 - gridSize / 2, 0.f, gridSize * 0.3f, gridSize * 0.3f) {
		Shader::setGlobalUniform("special", 0);
	}

	void update() {
		if(Input::isKeyPressed("ESCAPE"))
			window.close();

		if(endAnimation) {
			animTime += AppData::deltaT / animDuration;
			auto [winx, winy] = window.getWindowSize();
			auto t = animTime * animTime;
			float w, h;
			if(winx > winy) {
				w = lerp(winy/winx, 0.f, t);
				h = lerp(1.f, 0.f, t);
			} else {
				w = lerp(1.f, 0.f, t);
				h = lerp(winx/winy, 0.f, t);
			}
			Shader::setGlobalUniform("scale", w, h);
			Shader::setGlobalUniform("special", t);
			return;
		}

		player.update(obstacles.getObstacles());
		finish.update();

		if(player.collider.collides(finish.collider))
			endAnimation = true;
	}

	void render() {
		obstacles.render(player.collider);
		player.render();
		finish.render();

		window.endFrame();
	}

	void run() {
		std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeBetweenFrames{};
		double lag = 0.0;

		const auto timePerUpdate = AppData::data().timePerUpdate;

		while (!window.shouldClose()) {
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
};

void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length, [[maybe_unused]] const GLchar* message, [[maybe_unused]] const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		std::cerr << "GL Error: " << message << "\n";
}

int main() {
	initializeGLFW();
	Application app;

	glDebugMessageCallback(MessageCallback, nullptr);

	app.run();
}

