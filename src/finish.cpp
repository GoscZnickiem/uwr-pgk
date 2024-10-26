#include "finish.hpp"
#include "appdata.hpp"

#include <cmath>

Finish::Finish(float x, float y, float angle, float xScale, float yScale)
:	transform{x, y, angle, xScale, yScale}, collider(transform),
	m_vis(transform, AppData::data().modelFinish, AppData::data().shaderSingle) { }

void Finish::update() {
	static const float rotate = 0.5f * AppData::deltaT;
	transform.angle += rotate;
	collider.update();
}

void Finish::render() {
	m_vis.render();
}

