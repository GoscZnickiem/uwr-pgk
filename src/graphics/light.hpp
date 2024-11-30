#ifndef _GZN_PGK_GRAPHICS_LIGHT_
#define _GZN_PGK_GRAPHICS_LIGHT_

#include <glm/vec3.hpp>

struct Light {
	union {
		glm::vec3 position;
		glm::vec3 direction;
	};
	float padding;
	glm::vec3 color;
	float intensity;
};

#endif
