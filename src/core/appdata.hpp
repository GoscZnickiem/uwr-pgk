#ifndef _GZN_PGK_CORE_APPDATA_
#define _GZN_PGK_CORE_APPDATA_

#include "mainLoop.hpp"
#include "window.hpp"
#include "../graphics/renderer.hpp"
#include "../graphics/shader.hpp"
#include <array>
#include <cstdint>

struct AppData {

	MainLoop mainLoop;
	Window window;
	Renderer renderer;

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	struct {
		const Shader map2D;
		const Shader map3D;
	} shaders;

	static constexpr std::array<glm::vec3, 7> lightColors = {
		glm::vec3{1, 0, 1},
		glm::vec3{0.9f, 0.8f, 0.1f},
		glm::vec3{1, 0.5f, 1},
		glm::vec3{0, 0, 1},
		glm::vec3{0, 1, 1},
		glm::vec3{0, 1, 0}, 
		glm::vec3{1.05f, 1.f, 0.95f}
	};

	static inline uint64_t score = 0;

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
