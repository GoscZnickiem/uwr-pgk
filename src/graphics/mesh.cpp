#include "mesh.hpp"
#include <iostream>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(const std::vector<float>& vertexData, const std::vector<int>& indices)
: m_modelSize(static_cast<GLsizei>(indices.size())) {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_modelVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_modelVbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexData.size() * sizeof(float)), vertexData.data(), GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// indices
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(int)), indices.data(), GL_STATIC_DRAW);

	// instance transforms buffer
	glGenBuffers(1, &m_instanceVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_instancesReservedMemory/4), nullptr, GL_STATIC_DRAW);
	GLuint location = 3;
	for (GLuint i = 0; i < 4; i++) {
		glEnableVertexAttribArray(location + i);
		glVertexAttribPointer(location + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(i * 4 * sizeof(float)));
		glVertexAttribDivisor(location + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &m_modelVbo);
    glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_instanceVbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::bind() {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void Mesh::render() {
	// if(aaaa) std::cout << "render these " << m_instances << "\n";
	glDrawElementsInstanced(GL_TRIANGLES, m_modelSize, GL_UNSIGNED_INT, reinterpret_cast<void*>(0), m_instances);
}

void Mesh::setTransforms(glm::mat4* transforms, std::size_t count) {
	if(count == 0) return;
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceVbo);
	bool resized = false;
	m_instances = static_cast<GLsizei>(count);
	while(count * sizeof(glm::mat4) >= m_instancesReservedMemory) {
		resized = true;
		m_instancesReservedMemory *= 2;
	}
	if(resized) glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_instancesReservedMemory), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(count * sizeof(glm::mat4)), transforms);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setTransforms(std::vector<glm::mat4>& transforms) {
	setTransforms(transforms.data(), transforms.size());
}




static int getMidpoint(int v1, int v2, std::vector<glm::vec3>& vertices, std::unordered_map<std::size_t, int>& cache) {
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

Mesh Mesh::CreateSphereMesh(std::size_t subdivisions) {
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

	std::vector<float> vertexData;
	for(const auto& v : vertices) {
		vertexData.push_back(v.x);
		vertexData.push_back(v.y);
		vertexData.push_back(v.z);
		vertexData.push_back(v.x);
		vertexData.push_back(v.y);
		vertexData.push_back(v.z);
		vertexData.push_back(0.f);
		vertexData.push_back(0.f);
	}

	return Mesh(vertexData, indices);
}
