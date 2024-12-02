#ifndef _GZN_PGK_GRAPHICS_RENDERABLE_
#define _GZN_PGK_GRAPHICS_RENDERABLE_

#include "../transform.hpp"
#include "material.hpp"
#include "mesh.hpp"

struct Renderable {
	Transform* transform;
	Mesh* mesh;
	Material* material;
};

#endif
