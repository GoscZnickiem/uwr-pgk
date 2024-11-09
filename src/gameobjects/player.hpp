#ifndef _GZN_PGK_GAMEOBJECTS_PLAYER_
#define _GZN_PGK_GAMEOBJECTS_PLAYER_

// #include "../collisions/collisionTriangle.hpp"
#include "../model.hpp"
#include "obstacle.hpp"

#include <vector>

class Player {
public:
	Player();

	Transform transform;
	// CollisionTriangle collider;

	void update(const std::vector<Obstacle>& obstacles, float bound);
	void render();

private:
	Model m_vis;
};

#endif
