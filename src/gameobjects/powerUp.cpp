#include "powerUp.hpp"
#include "../appdata.hpp"

#include <cmath>
#include <random>

PowerUp::PowerUp(long unsigned int seed, float boardSize, glm::vec3 boardCenter)
:	transform{},
	m_vis(transform, AppData::data().modelPowerUp, AppData::data().shaderSingle) { 
	glm::vec3 v;
	const float board = boardSize / 2;
	static std::mt19937 gen(seed);
	std::uniform_real_distribution<float> dis(-board, board);
	do {
        v.x = dis(gen);
        v.y = dis(gen);
        v.z = dis(gen);
    } while (glm::length(v - glm::vec3(-board, -board, -board)) <= board/3 || 
			 glm::length(v - glm::vec3(board, board, board)) <=  board/3);
	transform.position = v + boardCenter;
}

void PowerUp::update() {
	static const float rotate = 1.5f * AppData::deltaT;
	transform.rotation.x += rotate * 1.3f;
	transform.rotation.z += rotate;
	transform.rotation.y += rotate * 1.7f;
}

void PowerUp::render() {
	m_vis.render();
}

