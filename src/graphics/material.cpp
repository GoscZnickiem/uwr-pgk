#include "material.hpp"

void Material::setup() const {
	shader->setUniform("ambient", ambient.r, ambient.g, ambient.b);
	shader->setUniform("diffuse", diffuse.r, diffuse.g, diffuse.b);
	shader->setUniform("specular", specular.r, specular.g, specular.b);
	shader->setUniform("shininess", shininess);
	shader->setUniform("opacity", opacity);
	shader->setUniform("alpha", alpha);
}
