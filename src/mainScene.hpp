#ifndef _GZN_PGK_MAINSCENE_
#define _GZN_PGK_MAINSCENE_

#include "heightMap.hpp"

#include <array>
#include <vector>

class MainScene {
public:
	MainScene();

	void update();
	void render();

	std::array<std::array<HeightMap, 181>, 361> chunks;
};

#endif
