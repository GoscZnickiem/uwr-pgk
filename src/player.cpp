#include "player.hpp"
#include "appdata.hpp"
#include "core/input.hpp"

Player::Player(float x, float y, float angle, float xScale, float yScale)
:	transform{x, y, angle, xScale, yScale}, 
	m_vis(transform, AppData::data().modelPlayer, AppData::data().shaderSingle) { }

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

