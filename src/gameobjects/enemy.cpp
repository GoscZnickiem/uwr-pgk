#include "enemy.hpp"
#include "../appdata.hpp"

#include <cmath>

inline float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

Enemy::Enemy()
:	transform{},
	m_vis(transform, AppData::data().modelEnemy, AppData::data().shaderSingle) {
	do {
        direction.x = randomFloat(-5.0f, 5.0f);
        direction.y = randomFloat(-5.0f, 5.0f);
        direction.z = randomFloat(-5.0f, 5.0f);
    } while (glm::length(direction - glm::vec3(-5.f, -5.f, -5.f)) <= 3 || glm::length(direction - glm::vec3(5.f, 5.f, 5.f)) <= 3);
	transform.position = -direction;
	direction = glm::normalize(direction);
}

void Enemy::update() {
	static const float rotate = 0.3f * AppData::deltaT;
	transform.rotation.x += rotate * 1.4f;
	transform.rotation.z += rotate;
	transform.rotation.y += rotate * 1.1f;

	static const float speed = 0.4f * AppData::deltaT;
	moveCycle += AppData::deltaT;
	if(moveCycle < 2.f) {
		transform.position += direction * speed;
	} else if(moveCycle >= 7.f && moveCycle < 9.f) {
		transform.position -= direction * speed;
	} else if(moveCycle >= 14.f) {
		moveCycle = 0;
	}
}

void Enemy::render() {
	m_vis.render();
}

