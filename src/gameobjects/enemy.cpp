#include "enemy.hpp"
#include "../appdata.hpp"

#include <cmath>
#include <random>

Enemy::Enemy(long unsigned int seed, float boardSize, glm::vec3 boardCenter)
:	transform{},
	m_vis(transform, AppData::data().modelEnemy, AppData::data().shaderSingle) {

	const float board = boardSize / 2;
	static std::mt19937 gen(seed);
	std::uniform_real_distribution<float> dis(-board, board);
	do {
        direction.x = dis(gen);
        direction.y = dis(gen);
        direction.z = dis(gen);
    } while (glm::length(direction - glm::vec3(-board, -board, -board)) <= board/3 || 
			 glm::length(direction - glm::vec3(board, board, board)) <=  board/3);
	transform.position = -direction + boardCenter;
	direction = glm::normalize(direction);
}

void Enemy::update() {
	static const float rotate = 0.3f * AppData::deltaT;
	transform.rotation.x += rotate * 1.4f;
	transform.rotation.z += rotate;
	transform.rotation.y += rotate * 1.1f;

	static const float speed = 0.1f * AppData::deltaT;
	moveCycle += AppData::deltaT;
	if(moveCycle < 2.f) {
		transform.position += direction * speed;
	} else if(moveCycle >= 4.f && moveCycle < 6.f) {
		transform.position -= direction * speed;
	} else if(moveCycle >= 8.f) {
		moveCycle = 0;
	}
}

void Enemy::render() {
	m_vis.render();
}

