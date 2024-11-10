#ifndef _GZN_PGK_GAMEOBJECTS_OBSTACLE_
#define _GZN_PGK_GAMEOBJECTS_OBSTACLE_

#include "../collisions/collisionPyramid.hpp"
#include "../transform.hpp"

class Obstacle {
public:
	Obstacle();

	Obstacle(const Obstacle& other) = delete;
	Obstacle operator=(const Obstacle& other) = delete;
	Obstacle(Obstacle&& other);
	Obstacle operator=(Obstacle&& other) = delete;

	Transform transform;
	CollisionPyramid collider;
};

#endif
