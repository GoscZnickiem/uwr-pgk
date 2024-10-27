#ifndef _GZN_PGK_GAMEOBJECTS_OBSTACLECOLLECTION_
#define _GZN_PGK_GAMEOBJECTS_OBSTACLECOLLECTION_

#include "../core/shader.hpp"
#include "../rawModel.hpp"
#include "obstacle.hpp"

#include <cstdint>
#include <vector>

class ObstacleCollection {
public:
	explicit ObstacleCollection(long unsigned seed, int size, float boardSize, float gridSize);
	~ObstacleCollection();

	[[nodiscard]] const std::vector<Obstacle>& getObstacles() const;

	void render(const CollisionTriangle& player);

private:
	const float m_boardSize;
	const float m_gridSize;
	const RawModel m_model;
	const Shader* m_shader;
	uint32_t m_vbo;

	std::vector<Obstacle> m_members;
};

#endif // !_GZN_PGK_GAMEOBJECTS_OBSTACLECOLLECTION_
