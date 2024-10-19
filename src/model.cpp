#include "model.hpp"

#include <GL/glew.h>

Model::Model(const std::vector<float>& vertices, const std::vector<int>& indices, const Shader& sh)
: shader(&sh), m_vertices(static_cast<int>(indices.size())) {
	p_createBuffers(vertices, indices);
}

Model::Model(const std::vector<float>& vertices, const Shader& sh) 
: shader(&sh), m_vertices(static_cast<int>(vertices.size()) / 2) {
	std::vector<int> indices;
	indices.reserve(vertices.size());
	for(int i = 0; i < m_vertices; i++)
		indices.push_back(i);
	p_createBuffers(vertices, indices);
}

Model::~Model() {
	glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Model::bind() const {
	if(m_vao == s_currentVAO) return;
	glBindVertexArray(m_vao);
	s_currentVAO = m_vao;
	shader->bind();
}

void Model::unbind() const {
	glBindVertexArray(0);
	s_currentVAO = 0;
	shader->unbind();
}

void Model::render() const {
	glDrawElements(GL_TRIANGLES, m_vertices, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
}

void Model::p_createBuffers(const std::vector<float>& vertices, const std::vector<int>& indices) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(int)), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
