#ifndef _GZN_PGK_CORE_APPDATA_
#define _GZN_PGK_CORE_APPDATA_

#include "mainLoop.hpp"
#include "window.hpp"

struct AppData {

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	MainLoop application;
	Window window;

	void atResize(int width, int height);

	static AppData& Data();

	static void Init();
	static void Terminate();

};

#endif
