#ifndef _GZN_PGK_GAMEOBJECTS_PLAYER_
#define _GZN_PGK_GAMEOBJECTS_PLAYER_

#include "../transform.hpp"
#include "../graphics/renderable.hpp"

class Player {
public:
	Player();

	Transform transform;
	Renderable visual;
	Light light;

	glm::vec3 direction;
	glm::vec3 aquariumBounds;

	void update();
	void render();
	void setLight();

	void rotatePitch(float rad);
	void rotateYaw(float rad);

	void setBounds(float width, float height, float depth);
};

#endif
