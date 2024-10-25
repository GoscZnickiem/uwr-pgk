#ifndef _GZN_PGK_OBSTACLE_
#define _GZN_PGK_OBSTACLE_

#include "transform.hpp"

class Obstacle {
public:
	Obstacle(float x, float y, float angle, float xSize = 1, float ySize = 1);

	Transform transform;
};

#endif
