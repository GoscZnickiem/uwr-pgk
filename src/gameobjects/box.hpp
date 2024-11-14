#ifndef _GZN_PGK_GAMEOBJECTS_BOX_
#define _GZN_PGK_GAMEOBJECTS_BOX_

#include "../model.hpp"

class Box {
public:
	Box(float size, glm::vec3 center);

	void render(float time);
private:
	Transform m_transform;
	Model m_vis;
};

#endif
