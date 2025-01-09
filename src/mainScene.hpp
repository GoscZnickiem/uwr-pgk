#ifndef _GZN_PGK_MAINSCENE_
#define _GZN_PGK_MAINSCENE_

#include "heightMap.hpp"

#include <vector>

class MainScene {
public:
	MainScene();

	void update();
	void render();

	std::vector<HeightMap> chunks;
};

#endif
