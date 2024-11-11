#ifndef _GZN_PGK_GAMEOBJECTS_ENEMY_
#define _GZN_PGK_GAMEOBJECTS_ENEMY_

#include "../model.hpp"

class Enemy {
public:
	explicit Enemy();

	Transform transform;

	void update();
	void render();

private:
	glm::vec3 direction;
	float moveCycle = 0;

	Model m_vis;
};

#endif
