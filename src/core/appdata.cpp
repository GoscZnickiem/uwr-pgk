#include "appdata.hpp"

#include <iostream>
#include <memory>

static std::unique_ptr<AppData> intance;

AppData& AppData::Data() {
	return *intance;
}

void AppData::Init() {
	if (glfwInit() == 0) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}
	intance = std::make_unique<AppData>(Token{});
}

void AppData::Terminate() {
	intance.reset();
	glfwTerminate();
}

void AppData::atResize(int width, int height) {
	glViewport(0, 0, width, height);
	sceneManager.atWindowResize(width, height);
	renderer.updateFrameBuffers(width, height);
}

AppData::AppData([[maybe_unused]] Token t) :
	mainLoop{}, window{}, sceneManager{}, renderer{},
	shaders {
		.map2D {"assets/shaders/map2D.glsl"},
		.map3D {"assets/shaders/map3D.glsl"}
	} { }
