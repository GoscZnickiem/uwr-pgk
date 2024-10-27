#include "core/window.hpp"
#include "core/input.hpp"
#include "appdata.hpp"
#include "gameobjects/background.hpp"
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

	BackGround backGround;
	ObstacleCollection obstacles;
	Player player;
	Finish finish;

	bool end = false;
	bool loading = true;
	float playTime = 0.f;
	static constexpr float animDuration = 3.f;
	float animTime = animDuration;

	Application(long unsigned int seed, int size)
		: gridSize(boardSize / static_cast<float>(size)), 
		  obstacles(seed, size, boardSize, gridSize),
		  player(-boardSize / 2 + gridSize / 2, -boardSize / 2 + gridSize / 2, -0.78f, gridSize * 0.1f, gridSize * 0.3f),
		  finish( boardSize / 2 - gridSize / 2,  boardSize / 2 - gridSize / 2, 0.f, gridSize * 0.3f, gridSize * 0.3f * 0.866f) {
		Shader::setGlobalUniform("special", 0);
	}

	void update() {
		playTime += AppData::deltaT;
		if(Input::isKeyPressed("ESCAPE") || animTime >= animDuration + 0.3f)
			window.close();

		finish.update();

		if(end || loading) {
			player.collider.update();
			animTime += (loading ? -2 : 1) * AppData::deltaT;
			if(animTime < 0.0f) {
				loading = false;
				animTime = 0.0f;
			}
			auto [winx, winy] = window.getWindowSize();
			auto t = (animTime / animDuration) * (animTime / animDuration);
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

		player.update(obstacles.getObstacles(), boardSize / 2);

		if(player.collider.collides(finish.collider))
			end = true;
	}

	void render() {
		backGround.render(playTime);
		obstacles.render(player.collider);
		player.render();
		finish.render();

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
			if(!end && !loading) timer += timeBetweenFrames.count();

			while(lag >= timePerUpdate ) {
				lag -= timePerUpdate;
				update();
			}

			render();
		}

		if(end) return timer;
		else return 0;
	}
};

void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length, [[maybe_unused]] const GLchar* message, [[maybe_unused]] const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		std::cerr << "GL Error: " << message << "\n";
}

int main (int argc, char *argv[]) {
	bool generatedSeed = true;
	unsigned long seed = static_cast<long unsigned int>(std::time(nullptr));
	int size = 10;
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

