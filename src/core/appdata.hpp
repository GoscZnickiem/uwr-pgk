#ifndef _GZN_PGK_CORE_APPDATA_
#define _GZN_PGK_CORE_APPDATA_

#include "mainLoop.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "sceneManager.hpp"
#include "../graphics/renderer.hpp"

struct AppData {

	MainLoop mainLoop;
	Window window;
	SceneManager sceneManager;
	Renderer renderer;

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	struct {
		const Shader standard;
	} shaders;

	struct {
		Mesh ball;
		Mesh cube;
		Mesh cubeInv;
		Mesh plane;
	} models;

	struct {
		Material bubble;
		Material test;
		Material glass;
		Material glass0;
		Material glass1;
		Material glass2;
		Material wood;
		Material aquariumBase;
		Material gravel;
	} materials;

	void atResize(int width, int height);

	static AppData& Data();
	static void Init();
	static void Terminate();

	AppData();

};

#endif
