#ifndef _GZN_PGK_GRAPHICS_MATERIAL_
#define _GZN_PGK_GRAPHICS_MATERIAL_

#include <glm/vec3.hpp>
#include "../core/shader.hpp"

struct Material {
	const Shader* shader;
	glm::vec3 ambient{1.f, 1.f, 1.f};
	glm::vec3 diffuse{1.f, 1.f, 1.f};
	glm::vec3 specular{1.f, 1.f, 1.f};
	float shininess{16.f};
	float opacity{1.f};
	float alpha{1.f};
	unsigned int textureID{0};
	int special{0};

	void setup() const;
};

#endif
