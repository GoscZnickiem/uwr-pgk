#ifndef _GZN_PGK_GAMEOBJECTS_FINISH_
#define _GZN_PGK_GAMEOBJECTS_FINISH_

#include "../collisions/collisionTriangle.hpp"
#include "../model.hpp"

class Finish {
public:
	explicit Finish(float x = 0, float y = 0, float angle = 0, float xScale = 1, float yScale = 1);

	Transform transform;
	CollisionTriangle collider;

	void update();
	void render();

private:
	Model m_vis;
};

#endif
