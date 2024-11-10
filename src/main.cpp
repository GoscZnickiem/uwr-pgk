#include "core/window.hpp"
#include "core/input.hpp"
#include "appdata.hpp"
#include "gameobjects/camera.hpp"
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
	Window window;

	const float boardSize = 10.0f;
	const float gridSize;

	Camera camera;
	Player player;
	ObstacleCollection obstacles;

	bool paused = false;

	Application(long unsigned int seed, int size)
	: window([this](float w, float h){ resizeCallback(w, h); }),
	gridSize(boardSize / static_cast<float>(size)),
	obstacles(seed, size, boardSize, gridSize) {
		const float playerCoord = -boardSize / 2 + gridSize / 2;
		const float playerScale = gridSize * 0.3f;
		player.transform.position = { playerCoord, playerCoord, playerCoord };
		player.transform.scale = { playerScale, playerScale, playerScale };
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Input::setMousePosLock(true);
		camera.update(player.transform.position);
	}

	void update() {
		if(!paused) {
			camera.update(player.transform.position);
			player.update(obstacles.getObstacles(), boardSize / 2);

			if(Input::isKeyClicked("ESCAPE")) {
				// window.close();
				Input::setMousePosLock(false);
				paused = true;
			}
		} else {
			if(Input::isKeyClicked("ESCAPE")) {
				Input::setMousePosLock(true);
				Input::getMousePos();
				paused = false;
			}
		}

		Input::update();
	}

	void render() {
		camera.setup();
		player.render();
		obstacles.render(); 

		window.endFrame();
	}

	double run() {
		std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeBetweenFrames{};
		const auto timePerUpdate = AppData::data().timePerUpdate;
		double lag = 0.0;

		double timer = 0.0;

		while (!window.shouldClose()) {
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

		return timer;
	}

	void resizeCallback(float w, float h) {
		std::cout << w << " " << h << "\n";
		camera.aspectRatio = w/h;
	}
};

void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length, [[maybe_unused]] const GLchar* message, [[maybe_unused]] const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		std::cerr << "GL Error: " << message << "\n";
}

int main (int argc, char *argv[]) {
	bool generatedSeed = true;
	unsigned long seed = static_cast<long unsigned int>(std::time(nullptr));
	int size = 5;
    if (argc > 3) {
        std::cerr << "Error: too many arguments. Proper usage: " << argv[0] << " [seed] [board size]\n";
        return 1;
    }
    if (argc >= 2) {
		try {
			seed = std::stoul(argv[1]);
			generatedSeed = false;
		} catch (std::invalid_argument& e) {
			if(argv[1][0] != 'r' || argv[1][1] != '\0') {
				std::cerr << "Error: argument for seed value should be a non-negative integer or 'r' for random value\n";
				return 1;
			}
		}
    }
    if (argc == 3) {
		size = std::stoi(argv[2]);
		if(size < 2) {
			std::cerr << "Error: size parameter should be at least 2\n";
			return 2;
		}
    }
	if(generatedSeed) {
		std::cout << "Seed: " << seed << "\n\n";
	}

	initializeGLFW();
	Application app(seed, size);

	glDebugMessageCallback(MessageCallback, nullptr);

	double time = app.run();

	if(time != 0) std::cout << "--== You win! ==--\nYour time: " << time << "s\n";
}

