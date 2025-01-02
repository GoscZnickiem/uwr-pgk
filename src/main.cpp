#include "core/appdata.hpp"
#include "core/window.hpp"
#include "heightMapReader.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <string_view>
#include <utility>

void GLAPIENTRY MessageCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei length, [[maybe_unused]] const GLchar* message, [[maybe_unused]] const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR)
		std::cerr << "GL Error: " << message << "\n";
}

int main (int argc, char** argv) {
	std::pair<int, int> latitude{-90,90};
	std::pair<int, int> longitude{-180,180};
	std::string directory;

    if (argc == 1) {
        std::cerr << "Error: Source directory not provided. Proper usage: " << argv[0] << " directory-path [options]\n";
        return 1;
    }
	for(int argIndex = 1; argIndex < argc; argIndex++) {
		char* arg = argv[argIndex];
		auto argString = std::string_view(arg);

		if(argString == "-h" || argString == "-help") {
			std::cout << "SEND HELP. HEEEEEEELP.\n";
			return 0;
		}

		auto coordOption = [&](std::string_view optName, std::pair<int,int>& dest, std::pair<int,int> range){
			auto end = [&](){
				std::cerr << "Error: Option usage: " << optName << " <minValue> <maxValue>\n"
					"Both values are expected in range [" << range.first << ", " << range.second << "]\n";
				exit(2);
			};
			if(argIndex + 2 >= argc) end();
			try {
				dest.first = std::stoi(argv[argIndex + 1]);
				dest.second = std::stoi(argv[argIndex + 2]);
			} catch (std::invalid_argument& e) {
				end();
			}
			if(dest.first > range.second || dest.first < range.first || dest.second > range.second || dest.second < range.first) end();
			argIndex += 2;
		};

		if(argString == "-lat") {
			coordOption("-lat", latitude, latitude);
		}
		else if(argString == "-lon") {
			coordOption("-lon", longitude, longitude);
		}
		else if(!directory.empty()) {
			std::cerr << "Error: Two directories provided - one expected\n";
			return 1;
		}
		else {
			directory = argString;
		}
	}

	AppData::Init();

	glDebugMessageCallback(MessageCallback, nullptr);

	
	std::cout << "Opening directory: " << directory << "\n";
	std::cout << "Lat: " << latitude.first << ", " << latitude.second << "\n";
	std::cout << "Lon: " << longitude.first << ", " << longitude.second << "\n";

	AppData::Data().mainScene.chunks = readData(directory, latitude, longitude);

	std::cout << "Read " << AppData::Data().mainScene.chunks.size() << " files\n";

	AppData::Data().mainLoop.run();

	AppData::Terminate();

	std::cout << "Exit app\n";
}
