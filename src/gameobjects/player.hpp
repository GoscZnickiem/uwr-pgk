#ifndef _GZN_PGK_GAMEOBJECTS_PLAYER_
#define _GZN_PGK_GAMEOBJECTS_PLAYER_

#include "../model.hpp"
#include "obstacle.hpp"

#include <vector>

struct KDTree;

class Player {
public:
	Player(const std::vector<Obstacle>& obs);
	~Player();

	Transform transform;

	void update(float bound, const glm::vec3& direction, const glm::vec3& up);
	void render();

	float poweredUp = 0;
private:
	Model m_vis;


	KDTree* tree;
	const std::vector<Obstacle>* obstacles;
};

#endif
