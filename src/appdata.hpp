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
	const Shader shaderBackground{"assets/shaders/shaderBackground.glsl"};

	const std::vector<float> modelObstacleV {
		-1.f, -1.f, -1.f,		0.40f, 0.40f, 0.40f,
		 0.f,  1.f, -1.f,		0.38f, 0.38f, 0.38f,
		 1.f, -1.f, -1.f,		0.36f, 0.36f, 0.36f,
		 0.f,  0.f,  1.f,		0.34f, 0.34f, 0.34f
	};
	const std::vector<int> modelObstacleI {
		0, 1, 2,
		3, 1, 0,
		3, 2, 1,
		0, 2, 3
	};

	const RawModel modelPlayer = RawModel::GenerateSphere(3, {1.f, 0.f, 0.f}, {1.f, 1.f, 0.f});

	const RawModel modelFinish = RawModel::GenerateSphere(3, {0.f, 1.f, 1.f}, {0.5f, 0.f, 1.f});

	const RawModel modelBox = {
		{
			-0.5f, -0.5f, -0.5f,	0.6f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.6f, 0.6f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.6f, 0.6f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, 0.6f, 0.0f,

			-0.5f, -0.5f, -0.5f,	0.7f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.7f, 0.7f, 0.0f,
			-0.5f,  0.5f, -0.5f,	0.7f, 0.7f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 0.7f, 0.3f,

			-0.5f, -0.5f,  0.5f,	0.7f, 0.7f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, 0.7f, 0.3f,
			-0.5f,  0.5f,  0.5f,	0.0f, 0.7f, 0.3f,
			 0.5f,  0.5f,  0.5f,	0.0f, 0.2f, 0.7f,

			-0.5f, -0.5f, -0.5f,	0.75f, 0.00f, 0.00f,
			-0.5f, -0.5f,  0.5f,	0.75f, 0.75f, 0.00f,
			-0.5f,  0.5f, -0.5f,	0.75f, 0.75f, 0.00f,
			-0.5f,  0.5f,  0.5f,	0.00f, 0.75f, 0.35f,

			 0.5f, -0.5f, -0.5f,	0.75f, 0.75f, 0.00f,
			 0.5f, -0.5f,  0.5f,	0.00f, 0.75f, 0.35f,
			 0.5f,  0.5f, -0.5f,	0.00f, 0.75f, 0.35f,
			 0.5f,  0.5f,  0.5f,	0.00f, 0.25f, 0.75f,

			-0.5f,  0.5f, -0.5f,	0.8f, 0.8f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 0.8f, 0.4f,
			-0.5f,  0.5f,  0.5f,	0.0f, 0.8f, 0.4f,
			 0.5f,  0.5f,  0.5f,	0.0f, 0.3f, 0.8f
		}, 
		{
			 0,  2,  1,  1,  2,  3,
			 4,  5,  6,  5,  7,  6,
			 8, 10,  9,  9, 10, 11,
			12, 14, 13, 13, 14, 15,
			16, 17, 18, 17, 19, 18,
			20, 21, 22, 21, 23, 22
		}
	};

	static AppData& data();

};

#endif
