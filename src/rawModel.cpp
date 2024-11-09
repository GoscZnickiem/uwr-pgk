#include "rawModel.hpp"

#include <GL/glew.h>
#include <cmath>
#include <glm/vec3.hpp>

RawModel::RawModel(const std::vector<float>& vertices, const std::vector<int>& indices)
: vertexCount(static_cast<int>(indices.size())) {
	p_createBuffers(vertices, indices);
}

RawModel::RawModel(const std::vector<float>& vertices) 
: vertexCount(static_cast<int>(vertices.size()) / 6) {
	std::vector<int> indices;
	indices.reserve(vertices.size());
	for(int i = 0; i < vertexCount; i++)
		indices.push_back(i);
	p_createBuffers(vertices, indices);
}

RawModel::~RawModel() {
	glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void RawModel::bind() const {
	if(m_vao == s_currentVAO) return;
	glBindVertexArray(m_vao);
	s_currentVAO = m_vao;
}

void RawModel::unbind() const {
	glBindVertexArray(0);
	s_currentVAO = 0;
}

void RawModel::p_createBuffers(const std::vector<float>& vertices, const std::vector<int>& indices) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(int)), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

RawModel RawModel::GenerateSphere([[maybe_unused]] std::size_t subdivisions) {
	const float t = (1.0f + std::sqrtf(5.0f)) / 2.0f;
	std::vector<glm::vec3> vertices = {
        {-1,  t,  0}, { 1,  t,  0}, {-1, -t,  0}, { 1, -t,  0},
        { 0, -1,  t}, { 0,  1,  t}, { 0, -1, -t}, { 0,  1, -t},
        { t,  0, -1}, { t,  0,  1}, {-t,  0, -1}, {-t,  0,  1}
    };

	std::vector<int> indices = {
        0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
        1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
        3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
        4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
    };


	std::vector<float> resVertices;
	for(const auto& v : vertices) {
		resVertices.push_back(v.x);
		resVertices.push_back(v.y);
		resVertices.push_back(v.z);
		resVertices.push_back(1.f);
		resVertices.push_back((v.y + 1.f)/2);
		resVertices.push_back(0.f);
	}

	return RawModel(resVertices, indices);
}
