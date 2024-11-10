#include "player.hpp"
#include "../appdata.hpp"
#include "../core/input.hpp"

#include <cmath>

Player::Player()
:	transform{},
	m_vis(transform, AppData::data().modelPlayer, AppData::data().shaderSingle) { }

void Player::update([[maybe_unused]] const std::vector<Obstacle>& obstacles, float bound, const glm::vec3& direction) {
	static const float speed = 10.0f * AppData::deltaT * transform.scale.x;

	if(Input::isKeyPressed("W")) {
		transform.position += speed * direction;
	}
	if(Input::isKeyPressed("S")) {
		transform.position -= speed * direction;
	}

	if(transform.position.x <= -bound) transform.position.x = -bound;
	else if(transform.position.x >= bound) transform.position.x = bound;
	if(transform.position.y <= -bound) transform.position.y = -bound;
	else if(transform.position.y >= bound) transform.position.y = bound;
	if(transform.position.z <= -bound) transform.position.z = -bound;
	else if(transform.position.z >= bound) transform.position.z = bound;

	for(const auto& o : obstacles) {
		auto collision = o.collider.collides(transform.position, transform.scale.x);
		if(collision) transform.position += *collision;
	}
}

void Player::render() {
	m_vis.render();
}

