#include "box.hpp"
#include "../appdata.hpp"

#include <cmath>

Box::Box(float size, glm::vec3 center)
:	m_transform{},
	m_vis(m_transform, AppData::data().modelBox, AppData::data().shaderBackground) { 
	m_transform.position = center;
	m_transform.scale = {size, size, size};
}

void Box::render(float time) {
	m_vis.m_shader->bind();
	m_vis.m_shader->setUniform("time", time);
	m_vis.render();
}

