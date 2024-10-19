#ifndef _GZN_PGK_PLAYER_
#define _GZN_PGK_PLAYER_

#include "drawable.hpp"

class Player {
public:
	Player(float x, float y);

	Transform transform;

	void update();
	void render();

private:
	Drawable m_vis;
};

#endif
