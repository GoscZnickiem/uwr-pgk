#ifndef _GZN_PGK_GRAPHICS_RENDERER_
#define _GZN_PGK_GRAPHICS_RENDERER_

#include "material.hpp"
#include "mesh.hpp"
#include "renderable.hpp"

#include <map>
#include <vector>

class Renderer {
public:
	Renderer();

	void render();
	void writeDepth();

	void addRender(Renderable renderData);

	void setCameraPos(const glm::vec3& pos);

	void updateFrameBuffers(int width, int height);

private:
	struct OpaqueRenderIndex {
		Mesh* mesh;
		Material* material;
		OpaqueRenderIndex(const Renderable& renderData);
		bool operator<(const OpaqueRenderIndex& other) const;
	};
	std::map<OpaqueRenderIndex, std::vector<glm::mat4>> opaqueBatches;
	std::vector<Renderable> translucentQueue;
	std::vector<glm::mat4> translucentArray;
	glm::vec3 cameraPosition;

	GLuint waterDepthTexture;
	GLuint frameBuffer;
};

#endif
