#include "powerUp.hpp"
#include "../appdata.hpp"

#include <cmath>

inline float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

PowerUp::PowerUp()
:	transform{},
	m_vis(transform, AppData::data().modelPowerUp, AppData::data().shaderSingle) { 
	glm::vec3 v;
	do {
        v.x = randomFloat(-5.0f, 5.0f);
        v.y = randomFloat(-5.0f, 5.0f);
        v.z = randomFloat(-5.0f, 5.0f);
    } while (glm::length(v - glm::vec3(-5.f, -5.f, -5.f)) <= 2 || glm::length(v - glm::vec3(5.f, 5.f, 5.f)) <= 4);
	transform.position = v;
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

