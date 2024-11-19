#include "appdata.hpp"

#include <iostream>
#include <memory>

static std::unique_ptr<AppData> intance;

AppData& AppData::Data() {
	return *intance;
}

void AppData::Init() {
	intance = std::make_unique<AppData>();
}

void AppData::Terminate() {
	intance.reset();
}

AppData::AppData() {
	if (glfwInit() == 0) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}
}

AppData::~AppData() {
	glfwTerminate();
}
