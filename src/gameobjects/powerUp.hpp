
#ifndef _GZN_PGK_GAMEOBJECTS_POWERUP_
#define _GZN_PGK_GAMEOBJECTS_POWERUP_

#include "../model.hpp"

class PowerUp {
public:
	PowerUp(long unsigned int seed, float boardSize, glm::vec3 boardCenter);

	Transform transform;

	void update();
	void render();

private:
	Model m_vis;
};

#endif
