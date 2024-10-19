#include "obstacle.hpp"
#include "appdata.hpp"

Obstacle::Obstacle(float x, float y, float angle, float xSize, float ySize)
: transform{x, y, angle, xSize, ySize}, m_vis(transform, AppData::data().modelObstacle) { }

void Obstacle::update() {}

void Obstacle::render() {
	m_vis.render();
}
