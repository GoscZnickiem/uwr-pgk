#include "box.hpp"
#include "../appdata.hpp"

#include <cmath>

Box::Box()
:	m_transform{},
	m_vis(m_transform, AppData::data().modelBox, AppData::data().shaderBackground) { 
	m_transform.scale = {10.f, 10.f, 10.f};
}

void Box::render(float time) {
	m_vis.m_shader->bind();
	m_vis.m_shader->setUniform("time", time);
	m_vis.render();
}

