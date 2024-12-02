#ifndef _GZN_PGK_GAMEOBJECTS_BUBBLE_
#define _GZN_PGK_GAMEOBJECTS_BUBBLE_

#include "../transform.hpp"
#include "../graphics/renderable.hpp"

class Bubble {
public:
	Bubble();

	Transform transform;
	Renderable visual;

	float speed;
	float startSize;
	float endSize;
	float startHeight;
	float endHeight;

	void update();
	void render();
};

#endif
