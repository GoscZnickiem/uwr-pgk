#include "player.hpp"
#include "../appdata.hpp"
#include "../core/input.hpp"

#include <cmath>

Player::Player()
:	transform{},
	m_vis(transform, AppData::data().modelPlayer, AppData::data().shaderSingle) { }

void Player::update([[maybe_unused]] const std::vector<Obstacle>& obstacles, float bound, const glm::vec3& direction) {
	static const float speed = 10.0f * AppData::deltaT * transform.scale.x;

	Transform prevTransform = transform;

	if(Input::isKeyPressed("W")) {
		transform.position += speed * direction;
	}
	if(Input::isKeyPressed("S")) {
		transform.position -= speed * direction;
	}

	// collider.update();
	//
	auto undo = [&]() {
		transform = prevTransform;
		// collider.update();
	};
	//
	// auto outOfBounds = [&](Point p) { return p.x < -bound || p.x > bound || p.y < -bound || p.y > bound; };
	if( transform.position.x <= -bound || transform.position.x >= bound || 
		transform.position.y <= -bound || transform.position.y >= bound || 
		transform.position.z <= -bound || transform.position.z >= bound) {
		undo();
		return;
	}
	// for(const auto& o : obstacles) {
	// 	if(!collider.collides(o.collider)) continue;
	// 	undo();
	// 	return;
	// }
}

void Player::render() {
	m_vis.render();
}

