#include "player.hpp"
#include "appdata.hpp"
#include "input.hpp"

Player::Player(float x, float y)
: transform{x, y, 0, 0.03f, 0.1f}, m_vis(transform, AppData::data().modelPlayer) { }

void Player::update() {
	if(Input::isKeyPressed("UP"))
		transform.y += 0.003f;
	if(Input::isKeyPressed("DOWN"))
		transform.y -= 0.003f;
	if(Input::isKeyPressed("LEFT"))
		transform.angle += 0.02f;
	if(Input::isKeyPressed("RIGHT"))
		transform.angle -= 0.02f;
}

void Player::render() {
	m_vis.render();
}

