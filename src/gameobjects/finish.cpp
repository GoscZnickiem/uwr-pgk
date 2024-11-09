#include "finish.hpp"
#include "../appdata.hpp"

#include <cmath>

Finish::Finish()
:	transform{},
	m_vis(transform, AppData::data().modelFinish, AppData::data().shaderSingle) { }

void Finish::update() {
	// static const float rotate = 0.5f * AppData::deltaT;
	// transform.angle += rotate;
	// collider.update();
}

void Finish::render() {
	m_vis.render();
}

