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

	const Shader shader{"assets/shaders/shader.glsl"};

	Mesh ball = Mesh::CreateSphereMesh(3);

	Material ballMat{
		.shader = &shader, .specular = glm::vec3{1.f, 0.f, 0.f}, .shininess = 32.f, .opacity = 0.8f
	};
	Material ballMat2{
		.shader = &shader, .shininess = 32.f, .opacity = 1.0f
	};

	void atResize(int width, int height);

	static AppData& Data();
	static void Init();
	static void Terminate();

};

#endif
