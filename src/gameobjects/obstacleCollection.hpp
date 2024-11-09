#ifndef _GZN_PGK_GAMEOBJECTS_OBSTACLECOLLECTION_
#define _GZN_PGK_GAMEOBJECTS_OBSTACLECOLLECTION_

#include "../instancedModel.hpp"
#include "obstacle.hpp"

#include <vector>

class ObstacleCollection {
public:
	explicit ObstacleCollection(long unsigned seed, int size, float boardSize, float gridSize);

	[[nodiscard]] const std::vector<Obstacle>& getObstacles() const;

	void render();

private:
	const float m_boardSize;
	const float m_gridSize;
	InstancedModel m_vis;

	std::vector<Obstacle> m_members;
};

#endif // !_GZN_PGK_GAMEOBJECTS_OBSTACLECOLLECTION_
