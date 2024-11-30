#include "material.hpp"

void Material::setup() const {
	// shader->setUniform("ambient", ambient.x, ambient.y, ambient.z);
	shader->setUniform("opacity", opacity);
}
