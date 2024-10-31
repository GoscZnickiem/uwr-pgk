#include "obstacleCollection.hpp"
#include "../appdata.hpp"

#include <cmath>
#include <ctime>
#include <random>
#include <GL/glew.h>

ObstacleCollection::ObstacleCollection(long unsigned seed, int size, float boardSize, float gridSize)
: m_boardSize(boardSize), m_gridSize(gridSize), m_model(AppData::data().modelObstacle), m_shader(&AppData::data().shaderInstanced) {

	m_members.reserve(static_cast<size_t>(size) * static_cast<size_t>(size));

	std::mt19937 gen(seed);
	std::uniform_real_distribution<float> angleDis(-3.14f, 3.14f);
	std::uniform_real_distribution<float> scaleDis(0.7f, 1.0f);

	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			if((i == 0 && j == 0) || (i == size - 1 && j == size - 1)) continue;
			const float scale = scaleDis(gen) * gridSize * 0.18f;
			m_members.emplace_back(
				static_cast<float>(i) * gridSize - boardSize / 2 + gridSize / 2,
				static_cast<float>(j) * gridSize - boardSize / 2 + gridSize / 2, 
				angleDis(gen), scale, scale * 3);
			m_members.back().collider.update();
		}
	}

	std::vector<float> vboData;
	vboData.reserve(m_members.size() * 6);
	for(const auto& m : m_members) {
		vboData.emplace_back(m.transform.x);
		vboData.emplace_back(m.transform.y);
		vboData.emplace_back(std::sinf(m.transform.angle));
		vboData.emplace_back(std::cosf(m.transform.angle));
		vboData.emplace_back(m.transform.xScale);
		vboData.emplace_back(m.transform.yScale);
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long int>(vboData.size() * sizeof(float)), vboData.data(), GL_STATIC_DRAW);

	m_model.bind();
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(4 * sizeof(float)));
	glVertexAttribDivisor(4, 1);
	m_model.unbind();
}

ObstacleCollection::~ObstacleCollection() {
    glDeleteBuffers(1, &m_vbo);
}

[[nodiscard]] const std::vector<Obstacle>& ObstacleCollection::getObstacles() const {
	return m_members;
}

void ObstacleCollection::render(const CollisionTriangle& player) {
	m_shader->bind();
	m_model.bind();
	m_shader->setUniform("playerPos1", player.a.x, player.a.y);
	m_shader->setUniform("playerPos2", player.b.x, player.b.y);
	m_shader->setUniform("playerPos3", player.c.x, player.c.y);
	glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, static_cast<int>(m_members.size()));
	m_model.unbind();
	m_shader->unbind();
}
