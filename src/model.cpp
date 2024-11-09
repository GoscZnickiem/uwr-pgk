#include "model.hpp"

#include <GL/gl.h>
#include <cmath>

Model::Model(Transform& transform, const RawModel& model, const Shader& shader)
: m_transform(&transform), m_model(&model), m_shader(&shader) { }

void Model::render() const {
	m_shader->bind();
	m_model->bind();

	m_shader->setUniform("model", m_transform->getMatrix());
	glDrawElements(GL_TRIANGLES, m_model->vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	m_model->unbind();
	m_shader->unbind();
}
