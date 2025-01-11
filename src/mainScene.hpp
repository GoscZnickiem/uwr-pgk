#ifndef _GZN_PGK_MAINSCENE_
#define _GZN_PGK_MAINSCENE_

#include "heightMap.hpp"

#include <array>
#include <glm/ext/vector_float2.hpp>

class MainScene {
public:
	MainScene();

	void update();
	void render();

	std::array<std::array<HeightMap, 180>, 360> chunks;
	glm::vec2 cameraPos;
	glm::vec2 scale;
	int areaXmin{0};
	int areaXmax{0};
	int areaYmin{0};
	int areaYmax{0};
};

#endif
