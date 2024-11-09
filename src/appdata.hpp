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

	const std::vector<float> modelObstacleV {
		-1.0f, -1.0f, -1.f,		1.0f, 0.0f, 0.0f,
		 0.0f,  1.0f, -1.f,		0.0f, 0.8f, 0.0f,
		 1.0f, -1.0f, -1.f,		0.2f, 0.3f, 1.0f,
		 0.0f,  0.0f,  1.f,		1.0f, 1.0f, 0.0f
	};
	const std::vector<int> modelObstacleI {
		0, 1, 2,
		0, 3, 1,
		2, 1, 3,
		0, 2, 3
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
