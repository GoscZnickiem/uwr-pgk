#ifndef _GZN_PGK_TRANSFORM_
#define _GZN_PGK_TRANSFORM_

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

struct Transform {
	glm::vec3 position{0.f};
	glm::vec3 rotation{0.f};
	glm::vec3 scale{1.f};

	glm::mat4 getMatrix() const;
};

#endif
