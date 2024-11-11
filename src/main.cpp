#include "core/window.hpp"
#include "core/input.hpp"
#include "appdata.hpp"
#include "gameobjects/camera.hpp"
#include "gameobjects/enemy.hpp"
#include "gameobjects/player.hpp"
#include "gameobjects/finish.hpp"
#include "gameobjects/box.hpp"
#include "gameobjects/obstacleCollection.hpp"
#include "gameobjects/powerUp.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>

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
	Camera minicamera;
	ObstacleCollection obstacles;
	Player player;
	Finish finish;
	Box box;
	std::vector<Enemy> enemies;
	std::vector<PowerUp> powerups;

	float time = 0.f;

	bool paused = false;
	bool outside = false;
	int visibilityMode = 1;

	Application(long unsigned int seed, int size)
		: window([this](int w, int h){ resizeCallback(w, h); }),
		gridSize(boardSize / static_cast<float>(size)),
		obstacles(seed, size, boardSize, gridSize),
		player(obstacles.getObstacles()) {

		const float playerCoord = -boardSize / 2 + gridSize / 2;
		const float playerScale = gridSize * 0.3f;
		player.transform.position = { playerCoord, playerCoord, playerCoord };
		player.transform.scale = { playerScale, playerScale, playerScale };
		finish.transform.position = { -playerCoord, -playerCoord, -playerCoord };
		finish.transform.scale = { playerScale, playerScale, playerScale };

		const std::size_t enemyCount = static_cast<std::size_t>((size - 2) * (size - 2));
		enemies.reserve(enemyCount);
		for(std::size_t i = 0; i < enemyCount; i++) {
			enemies.emplace_back();
			enemies.back().transform.scale = { playerScale, playerScale, playerScale };
		}

		const std::size_t powerupCount = static_cast<std::size_t>((size - 2) * 4);
		powerups.reserve(powerupCount);
		for(std::size_t i = 0; i < powerupCount; i++) {
			powerups.emplace_back();
			powerups.back().transform.scale = { playerScale * 0.7f, playerScale * 0.7f, playerScale * 0.7f };
		}

		camera.update(player.transform.position);

		minicamera.viewSize = { 0.16f * 2, 0.9f * 2};
		minicamera.viewPos = { 0.7f, 0.8f };
		minicamera.outsideMode = true;
		minicamera.update(player.transform.position);

		Input::setMousePosLock(true);
	}

	void update() {
		if(!paused) {
			if(outside) minicamera.update(player.transform.position);
			else camera.update(player.transform.position);
			player.update(boardSize/2, camera.direction, camera.up);
			finish.update();
			for(auto& p : powerups) p.update();
			for(auto& e : enemies) e.update();

			if(Input::isKeyClicked("SPACE")) {
				visibilityMode = (visibilityMode + 1) % 3;
				minicamera.ortoMode = visibilityMode == 2;
			}

			if(Input::isKeyClicked("C")) {
				auto miniSizeP = minicamera.viewSizeP;
				auto miniPosP = minicamera.viewPosP;
				auto miniSize = minicamera.viewSize;
				auto miniPos = minicamera.viewPos;
				minicamera.viewSizeP = camera.viewSizeP;
				minicamera.viewPosP = camera.viewPosP;
				minicamera.viewSize = camera.viewSize;
				minicamera.viewPos = camera.viewPos;
				camera.viewSizeP = miniSizeP;
				camera.viewPosP = miniPosP;
				camera.viewSize = miniSize;
				camera.viewPos = miniPos;
				outside = !outside;
			}

			if(outside && visibilityMode == 0) visibilityMode = 1; 

			if(glm::length(player.transform.position - finish.transform.position) <= player.transform.scale.x + player.transform.scale.x / 1.5f) {
				window.close();
			}

			for(auto& p : powerups)
				if(glm::length(player.transform.position - p.transform.position) <= player.transform.scale.x + p.transform.scale.x / 1.5f) {
					player.poweredUp = 3.f;
					p.transform.position.x += 99999.f;
				}

			for(auto& e : enemies)
				if(glm::length(player.transform.position - e.transform.position) <= player.transform.scale.x + e.transform.scale.x / 1.8f)
					reset();

			if(Input::isKeyClicked("ESCAPE")) {
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
	
		time += AppData::deltaT;
		Input::update();
	}

	void render() {

		if(!outside) {
			camera.setup();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			player.render();
			obstacles.render(time); 
			finish.render();
			box.render(time);
			for(auto& p : powerups) p.render();
			for(auto& e : enemies) e.render();
		}

		if(visibilityMode != 0) {
			minicamera.setup();
			glPolygonMode(GL_FRONT_AND_BACK, visibilityMode == 2 ? GL_LINE : GL_FILL);
			glClear(GL_DEPTH_BUFFER_BIT);
			player.render();
			obstacles.render(time); 
			finish.render();
			box.render(time);
			for(auto& p : powerups) p.render();
			for(auto& e : enemies) e.render();
		}

		window.endFrame();
	}

	void reset() {
		const float playerCoord = -boardSize / 2 + gridSize / 2;
		player.transform.position = { playerCoord, playerCoord, playerCoord };
		player.poweredUp = 0;

		for(auto& p : powerups)
		if(p.transform.position.x >= 99990.f) {
			p.transform.position.x -= 99999.f;
		}
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

	void resizeCallback(int w, int h) {
		camera.updateResolution(w, h);
		minicamera.updateResolution(w, h);
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

