#ifndef _GZN_PGK_GAMEOBJECTS_BACKGROUND_
#define _GZN_PGK_GAMEOBJECTS_BACKGROUND_

#include "../core/shader.hpp"

#include <cstdint>

class BackGround {
public:
	BackGround();
	~BackGround();

	void render(float time);
private:
	uint32_t m_vao;
	const Shader* m_shader;
};


#endif // !_GZN_PGK_GAMEOBJECTS_BACKGROUND_
