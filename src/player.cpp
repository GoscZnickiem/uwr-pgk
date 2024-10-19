#include "player.hpp"
#include "appdata.hpp"

Player::Player(float x, float y)
: transform{x, y, 0, 0.03f, 0.1f}, m_vis(transform, AppData::data().modelPlayer) { }

void Player::update() {}

void Player::render() {
	m_vis.render();
}

