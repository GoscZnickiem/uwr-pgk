#ifndef _GZN_PGK_PLAYER_
#define _GZN_PGK_PLAYER_

#include "drawable.hpp"

class Player {
public:
	explicit Player(float x = 0, float y = 0, float angle = 0, float xScale = 1, float yScale = 1);

	Transform transform;

	void update();
	void render();

private:
	Drawable m_vis;
};

#endif
