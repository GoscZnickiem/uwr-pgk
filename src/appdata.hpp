#ifndef _GZN_PGK_APPDATA_
#define _GZN_PGK_APPDATA_

#include "rawModel.hpp"
#include "core/shader.hpp"

#include <string>
#include <vector>

struct AppData {

	static constexpr unsigned int updatesPerSecond = 256;
	static constexpr double timePerUpdate = 1./updatesPerSecond;
	static constexpr float deltaT = static_cast<float>(timePerUpdate);

	const Shader shaderSingle{"assets/shaders/shaderSingle.glsl"};
	const Shader shaderInstanced{"assets/shaders/shaderInstanced.glsl"};
	// const Shader shaderBackground{"assets/shaders/shaderBackground.glsl"};

	const std::vector<float> modelObstacle {
		-1.0f, -1.0f,	0.4f, 0.1f, 0.7f,
		 0.0f,  1.0f,	0.1f, 0.5f, 0.7f,
		 1.0f, -1.0f,	0.4f, 0.1f, 0.7f
	};

	const RawModel modelPlayer = RawModel::GenerateSphere();

	const RawModel modelFinish { {
		-1.0f, -1.0f,	1.0f, 1.0f, 0.2f,
		 0.0f,  1.0f,	0.3f, 0.3f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.3f, 0.3f,
	} };

	static AppData& data();

};

#endif
