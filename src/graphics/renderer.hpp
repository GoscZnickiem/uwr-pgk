#ifndef _GZN_PGK_GRAPHICS_RENDERER_
#define _GZN_PGK_GRAPHICS_RENDERER_

#include "material.hpp"
#include "mesh.hpp"
#include "renderable.hpp"

#include <map>
#include <vector>

class Renderer {
public:
	void render();

	void addRender(Renderable renderData);

	void setCameraPos(const glm::vec3& pos);

private:
	struct RenderDataIndex {
		Mesh* mesh;
		Material* material;
		RenderDataIndex(const Renderable& renderData);
		bool operator<(const RenderDataIndex& other) const;
	};
	std::map<RenderDataIndex, std::vector<glm::mat4>> batches;
	glm::vec3 cameraPosition;
};

#endif
