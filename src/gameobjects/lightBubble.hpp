#ifndef _GZN_PGK_GAMEOBJECTS_LIGHTBUBBLE_
#define _GZN_PGK_GAMEOBJECTS_LIGHTBUBBLE_

#include "../graphics/light.hpp"
#include "bubble.hpp"

class LightBubble : public Bubble {
public:
	LightBubble(glm::vec3 color);

	Light light;

	void update();
	void render();
	void setLight();
};

#endif
