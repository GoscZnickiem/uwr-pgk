#include "player.hpp"
#include "appdata.hpp"
#include "core/input.hpp"

#include <cmath>

Player::Player(float x, float y, float angle, float xScale, float yScale)
:	transform{x, y, angle, xScale, yScale}, collider(transform),
	m_vis(transform, AppData::data().modelPlayer, AppData::data().shaderSingle) { }

void Player::update(const std::vector<Obstacle>& obstacles) {
	static constexpr float speed = 0.3f * AppData::deltaT;
	static constexpr float rotate = 1.5f * AppData::deltaT;

	float prevX = transform.x;
	float prevY = transform.y;
	float prevAngle = transform.angle;

	if(Input::isKeyPressed("UP")) {
		transform.x -= speed * std::sinf(transform.angle);
		transform.y += speed * std::cosf(transform.angle);
	}
	if(Input::isKeyPressed("DOWN")) {
		transform.x += speed * std::sinf(transform.angle);
		transform.y -= speed * std::cosf(transform.angle);
	}
	if(Input::isKeyPressed("LEFT"))
		transform.angle += rotate;
	if(Input::isKeyPressed("RIGHT"))
		transform.angle -= rotate;

	collider.update();
	for(const auto& o : obstacles) {
		if(!collider.isNear(o.collider)) continue;
		if(!collider.collides(o.collider)) continue;
		transform.x = prevX;
		transform.y = prevY;
		transform.angle = prevAngle;
		collider.update();
		return;
	}
}

void Player::render() {
	m_vis.render();
}

