#include "instancedModel.hpp"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

InstancedModel::InstancedModel(const std::vector<float>& vertices, const std::vector<int>& indices, const Shader& shader)
: m_model(vertices, indices), m_shader(&shader) { }

InstancedModel::InstancedModel(const std::vector<float>& vertices, const Shader& shader)
: m_model(vertices), m_shader(&shader) { }

InstancedModel::~InstancedModel() {
    glDeleteBuffers(1, &m_vbo);
}

void InstancedModel::instantiate(const std::vector<glm::mat4>& transforms) {
	m_instances = static_cast<int>(transforms.size());

	std::vector<float> vboData;
	vboData.reserve(transforms.size() * 16);
	for(const auto& m : transforms) {
		const float* matPtr = glm::value_ptr(m);
		vboData.insert(vboData.end(), matPtr, matPtr + 16);
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long int>(vboData.size() * sizeof(float)), vboData.data(), GL_STATIC_DRAW);

	m_model.bind();
	GLuint location = 2;
	for (GLuint i = 0; i < 4; i++) {
		glEnableVertexAttribArray(location + i);
		glVertexAttribPointer(location + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), reinterpret_cast<void*>(i * 4 * sizeof(float)));
		glVertexAttribDivisor(location + i, 1);
	}
	m_model.unbind();
}

void InstancedModel::render() const {
	m_shader->bind();
	m_model.bind();
	glDrawElementsInstanced(GL_TRIANGLES, m_model.vertexCount, GL_UNSIGNED_INT, 0, m_instances);
	m_model.unbind();
	m_shader->unbind();
}
