#include "drawable.hpp"

#include <cmath>

Drawable::Drawable(Transform& transform, const Model& model) : m_transform(&transform), m_model(&model) { }

void Drawable::render() const {
	m_model->bind();
	m_model->shader->setUniform("pos", m_transform->x, m_transform->y);
	m_model->shader->setUniform("sin", std::sinf(m_transform->angle));
	m_model->shader->setUniform("cos", std::cosf(m_transform->angle));
	m_model->shader->setUniform("scale", m_transform->xScale, m_transform->yScale);
	m_model->render();
}
