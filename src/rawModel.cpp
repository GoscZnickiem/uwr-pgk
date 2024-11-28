#include "rawModel.hpp"

#include <GL/glew.h>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>

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

int getMidpoint(int v1, int v2, std::vector<glm::vec3>& vertices, std::unordered_map<std::size_t, int>& cache) {
	std::size_t smallerIndex = static_cast<std::size_t>(v1 < v2 ? v1 : v2);
	std::size_t greaterIndex = static_cast<std::size_t>(v1 < v2 ? v2 : v1);
	std::size_t key = (smallerIndex << 32) + greaterIndex;

    auto it = cache.find(key);
    if (it != cache.end()) return it->second;

	glm::vec3 vertex1 = vertices[static_cast<std::size_t>(v1)];
	glm::vec3 vertex2 = vertices[static_cast<std::size_t>(v2)];
	glm::vec3 midpoint = (vertex1 + vertex2) / 2.0f;
    
	vertices.push_back(glm::normalize(midpoint));
    int midIndex = static_cast<int>(vertices.size() - 1);
    cache[key] = midIndex;
    return midIndex;
}

static glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
	return (1 - t) * a + t * b;
}

RawModel RawModel::GenerateSphere(std::size_t subdivisions, const glm::vec3& gradIn, const glm::vec3& gradOut) {
	const float t = (1.0f + std::sqrtf(5.0f)) / 2.0f;
	std::vector<glm::vec3> vertices = {
        {-1,  t,  0}, { 1,  t,  0}, {-1, -t,  0}, { 1, -t,  0},
        { 0, -1,  t}, { 0,  1,  t}, { 0, -1, -t}, { 0,  1, -t},
        { t,  0, -1}, { t,  0,  1}, {-t,  0, -1}, {-t,  0,  1}
    }; // icosahedron
	for(auto& v : vertices) {
		v = glm::normalize(v);
	}

	std::vector<int> indices = {
        0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
        1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
        3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
        4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
    };

	std::unordered_map<std::size_t, int> midpointCache;
    for (std::size_t i = 0; i < subdivisions; i++) {
        std::vector<int> newIndices;
        for (size_t j = 0; j < indices.size(); j += 3) {
            int v1 = indices[j];
            int v2 = indices[j + 1];
            int v3 = indices[j + 2];

            int a = getMidpoint(v1, v2, vertices, midpointCache);
            int b = getMidpoint(v2, v3, vertices, midpointCache);
            int c = getMidpoint(v3, v1, vertices, midpointCache);

            newIndices.push_back(v1); newIndices.push_back(a); newIndices.push_back(c);
            newIndices.push_back(v2); newIndices.push_back(b); newIndices.push_back(a);
            newIndices.push_back(v3); newIndices.push_back(c); newIndices.push_back(b);
            newIndices.push_back(a); newIndices.push_back(b); newIndices.push_back(c);
        }
        indices = newIndices;
    }

	std::vector<float> resVertices;
	for(const auto& v : vertices) {
		glm::vec3 color = lerp(gradIn, gradOut, (v.y + 1.f)/2);
		resVertices.push_back(v.x);
		resVertices.push_back(v.y);
		resVertices.push_back(v.z);
		resVertices.push_back(color.r);
		resVertices.push_back(color.g);
		resVertices.push_back(0.f);
	}

	return RawModel(resVertices, indices);
}
