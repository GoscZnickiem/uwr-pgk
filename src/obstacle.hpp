#ifndef _GZN_PGK_OBSTACLE_
#define _GZN_PGK_OBSTACLE_

#include "drawable.hpp"

class Obstacle {
public:
	Obstacle(float x, float y, float angle, float xSize = 1, float ySize = 1);

	Transform transform;

	void update();
	void render();

private:
	Drawable m_vis;
};

#endif
