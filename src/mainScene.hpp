#ifndef _GZN_PGK_MAINSCENE_
#define _GZN_PGK_MAINSCENE_

#include "camera.hpp"
#include "heightMap.hpp"

#include <array>
#include <glm/ext/vector_float2.hpp>

class MainScene {
public:
	MainScene();

	void update();
	void render();
	void atResize(int width, int height);

	bool view2D{true};

	std::array<std::array<HeightMap, 180>, 360> chunks;
	glm::vec2 cameraPos;
	float scale;
	glm::vec2 aspectRatio;
	int areaXmin{0};
	int areaXmax{0};
	int areaYmin{0};
	int areaYmax{0};

	float cameraHeight;
	Camera camera;
};

#endif
