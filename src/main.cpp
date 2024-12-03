#include "core/appdata.hpp"
#include "core/window.hpp"
#include "scenes/level.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>

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

	AppData::Init();

	glDebugMessageCallback(MessageCallback, nullptr);

	AppData::Data().sceneManager.pushScene<Level>(30);
	AppData::Data().sceneManager.switchToNextScene();
	AppData::Data().mainLoop.run();

	AppData::Terminate();
}
