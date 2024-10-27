#ifndef _GZN_PGK_GAMEOBJECTS_PLAYER_
#define _GZN_PGK_GAMEOBJECTS_PLAYER_

#include "../collisions/collisionTriangle.hpp"
#include "../model.hpp"
#include "obstacle.hpp"

#include <vector>

class Player {
public:
	explicit Player(float x = 0, float y = 0, float angle = 0, float xScale = 1, float yScale = 1);

	Transform transform;
	CollisionTriangle collider;

	void update(const std::vector<Obstacle>& obstacles);
	void render();

private:
	Model m_vis;
};

#endif
