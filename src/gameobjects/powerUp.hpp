
#ifndef _GZN_PGK_GAMEOBJECTS_POWERUP_
#define _GZN_PGK_GAMEOBJECTS_POWERUP_

#include "../model.hpp"

class PowerUp {
public:
	explicit PowerUp();

	Transform transform;

	void update();
	void render();

private:
	Model m_vis;
};

#endif
