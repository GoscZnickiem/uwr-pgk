#ifndef _GZN_PGK_GAMEOBJECTS_FINISH_
#define _GZN_PGK_GAMEOBJECTS_FINISH_

// #include "../collisions/collisionTriangle.hpp"
#include "../model.hpp"

class Finish {
public:
	explicit Finish();

	Transform transform;
	// CollisionTriangle collider;

	void update();
	void render();

private:
	Model m_vis;
};

#endif
