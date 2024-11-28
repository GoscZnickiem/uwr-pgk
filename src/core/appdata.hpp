#ifndef _GZN_PGK_CORE_APPDATA_
#define _GZN_PGK_CORE_APPDATA_

#include "mainLoop.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "sceneManager.hpp"

struct AppData {

	MainLoop mainLoop;
	Window window;
	SceneManager sceneManager;

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	const Shader shader{"assets/shaders/shader.glsl"};

	void atResize(int width, int height);

	static AppData& Data();

	static void Init();
	static void Terminate();

};

#endif
