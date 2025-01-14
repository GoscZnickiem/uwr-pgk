#include "core/appdata.hpp"
#include "core/window.hpp"

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
	std::pair<float, float> pos{0,0};
	std::size_t lod = 0;
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
		else if(argString == "-lod") {
			auto end = [&](){
				std::cerr << "Error: Option usage: -lod <lodValue>\n"
					"The value is either 0 - for auto-lod or in range 1-9 where 1 means best quality and 9 means worst quality\n";
				exit(2);
			};
			if(argIndex + 1 >= argc) end();
			try {
				lod = std::stoull(argv[argIndex + 1]);
			} catch (std::invalid_argument& e) {
				end();
			}
			if(lod > HeightMap::LODS) end();
			argIndex ++;
		}
		else if(argString == "-pos") {
			auto end = [&](){
				std::cerr << "Error: Option usage: -pos <latitude> <longitude>\n"
					"Values are expected in range [-90, 90] and [-180, 180] respectively\n";
				exit(2);
			};
			if(argIndex + 2 >= argc) end();
			try {
				pos.second = std::stof(argv[argIndex + 1]);
				pos.first = std::stof(argv[argIndex + 2]);
			} catch (std::invalid_argument& e) {
				end();
			}
			if(pos.first > 90 || pos.first < -90 || pos.second > 180 || pos.second < -180) end();
			argIndex += 2;
		}
		else if(!directory.empty()) {
			std::cerr << "Error: Two directories provided - one expected\n";
			return 1;
		}
		else {
			directory = argString;
		}
	}

	AppData::SetReadDirectory(directory);
	AppData::latBounds = {latitude.first + 90, latitude.second + 90};
	AppData::lonBounds = {longitude.first + 180, longitude.second + 180};
	AppData::lod = lod;
	AppData::Init();
	AppData::Data().mainScene.cameraPos = {pos.first, pos.second};
	AppData::Data().window.uselessMethod();

	glDebugMessageCallback(MessageCallback, nullptr);

	AppData::Data().mainLoop.run();

	AppData::Terminate();
}
