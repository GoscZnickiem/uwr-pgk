#include "material.hpp"

#include <GL/glew.h>

void Material::setup() const {
	shader->setUniform("ambient", ambient.r, ambient.g, ambient.b);
	shader->setUniform("diffuse", diffuse.r, diffuse.g, diffuse.b);
	shader->setUniform("specular", specular.r, specular.g, specular.b);
	shader->setUniform("shininess", shininess);
	shader->setUniform("opacity", opacity);
	shader->setUniform("alpha", alpha);
	shader->setUniform("special", special);
	if(special == 2) {
		glDisable(GL_CULL_FACE); 
	} else {
		glEnable(GL_CULL_FACE); 
	}
}
