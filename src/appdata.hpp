#ifndef _GZN_PGK_APPDATA_
#define _GZN_PGK_APPDATA_

#include "model.hpp"
#include "shader.hpp"

#include <string>

struct AppData {

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	const Shader shaderBasic{"assets/shaders/shader.glsl"};

	const Model modelObstacle{ {
		-1.0f, -1.0f,	0.5f, 0.1f, 0.8f,
		 0.0f,  1.0f,	0.2f, 0.6f, 0.8f,
		 1.0f, -1.0f,	0.5f, 0.1f, 0.8f
	}, shaderBasic};

	const Model modelPlayer{ {
		-1.0f, -1.0f,	0.7f, 0.7f, 0.1f,
		 0.0f,  1.0f,	0.8f, 0.1f, 0.0f,
		 1.0f, -1.0f,	0.7f, 0.7f, 0.1f,
	}, shaderBasic};

	static AppData& data();

};

#endif
