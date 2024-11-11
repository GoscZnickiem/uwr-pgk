#include "obstacleCollection.hpp"
#include "../appdata.hpp"

#include <cmath>
#include <ctime>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <GL/glew.h>

ObstacleCollection::ObstacleCollection(long unsigned seed, int size, float boardSize, float gridSize)
: m_boardSize(boardSize), m_gridSize(gridSize), m_vis(AppData::data().modelObstacleV, AppData::data().modelObstacleI, AppData::data().shaderInstanced) {

	m_members.reserve(static_cast<size_t>(size) * static_cast<size_t>(size) * static_cast<size_t>(size));

	std::mt19937 gen(seed);
	std::uniform_real_distribution<float> angleDis(-3.14f, 3.14f);
	std::uniform_real_distribution<float> scaleDis(0.6f, 1.0f);

	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			for(int k = 0; k < size; k++) {
				if((i == 0 && j == 0 && k == 0) || (i == size - 1 && j == size - 1 && k == size - 1)) continue;
				const float scale = scaleDis(gen) * gridSize * 0.4f;
				auto& m = m_members.emplace_back();
				m.transform.position = {
					static_cast<float>(i) * gridSize - boardSize / 2 + gridSize / 2,
					static_cast<float>(j) * gridSize - boardSize / 2 + gridSize / 2, 
					static_cast<float>(k) * gridSize - boardSize / 2 + gridSize / 2
				};
				m.transform.rotation = { angleDis(gen), angleDis(gen), angleDis(gen) };
				m.transform.scale = {scale, scale, scale * 2};
				m.collider.update();
			}
		}
	}

	std::vector<glm::mat4> transforms;
	transforms.reserve(m_members.size());
	for(auto& m : m_members) {
		transforms.emplace_back(m.transform.getMatrix());
	}

	m_vis.instantiate(transforms);
}

[[nodiscard]] const std::vector<Obstacle>& ObstacleCollection::getObstacles() const {
	return m_members;
}

void ObstacleCollection::render(float time) {
	m_vis.m_shader->bind();
	m_vis.m_shader->setUniform("time", time);
	m_vis.render();
}
