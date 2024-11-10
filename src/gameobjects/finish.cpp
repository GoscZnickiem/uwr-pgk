#include "finish.hpp"
#include "../appdata.hpp"

#include <cmath>

Finish::Finish()
:	transform{},
	m_vis(transform, AppData::data().modelFinish, AppData::data().shaderSingle) { }

void Finish::update() {
	static const float rotate = 1.5f * AppData::deltaT;
	transform.rotation.x += rotate;
	transform.rotation.z += rotate;
}

void Finish::render() {
	m_vis.render();
}

