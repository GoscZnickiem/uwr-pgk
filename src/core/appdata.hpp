#ifndef _GZN_PGK_CORE_APPDATA_
#define _GZN_PGK_CORE_APPDATA_

#include "mainLoop.hpp"
#include "window.hpp"
#include "shader.hpp"

struct AppData {

	MainLoop mainLoop;
	Window window;
	MainScene mainScene;

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	struct {
		const Shader map2D;
		const Shader map3D;
	} shaders;

	void atResize(int width, int height);

	static AppData& Data();
	static void Init();
	static void Terminate();

private:
	struct Token {};
public:

	AppData(Token t);

};

#endif
