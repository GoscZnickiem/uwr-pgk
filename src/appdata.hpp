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
		-1.f, -1.f, -1.f,		0.2f, 0.70f, 0.2f,
		 0.f,  1.f, -1.f,		0.4f, 0.85f, 0.3f,
		 1.f, -1.f, -1.f,		0.2f, 0.80f, 0.3f,
		 0.f,  0.f,  1.f,		0.1f, 0.65f, 0.2f
	};
	const std::vector<int> modelObstacleI {
		0, 1, 2,
		3, 1, 0,
		3, 2, 1,
		0, 2, 3
	};

	const RawModel modelPlayer = RawModel::GenerateSphere(3, {1.f, 0.f, 0.f}, {1.f, 1.f, 0.f});

	const RawModel modelFinish = RawModel::GenerateSphere(3, {0.f, 1.f, 1.f}, {0.5f, 0.f, 1.f});

	static AppData& data();

};

#endif
