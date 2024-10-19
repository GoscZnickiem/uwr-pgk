#include "drawable.hpp"

#include <cmath>

Drawable::Drawable(const Model& model) : x{}, y{}, angle{}, xScale{1}, yScale{1}, m_model(&model) { }

void Drawable::render() const {
	m_model->bind();
	m_model->shader->setUniform("pos", x, y);
	m_model->shader->setUniform("sin", std::sinf(angle));
	m_model->shader->setUniform("cos", std::cosf(angle));
	m_model->shader->setUniform("scale", xScale, yScale);
	m_model->render();
}
