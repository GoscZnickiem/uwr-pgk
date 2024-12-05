#ifndef _GZN_PGK_GRAPHICS_MESH_
#define _GZN_PGK_GRAPHICS_MESH_

#include <GL/glew.h>
#include <cstddef>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

class Mesh {
public:
	Mesh(const std::vector<float>& vertexData, const std::vector<int>& indices);
	~Mesh();
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(Mesh&& other) noexcept = delete;
	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	void bind();
	void render();

	void setTransforms(glm::mat4* transforms, std::size_t count);
	void setTransforms(std::vector<glm::mat4>& transforms);

private:
	GLuint m_vao;
	GLuint m_modelVbo;
	GLuint m_ebo;
	GLuint m_instanceVbo;
	GLsizei m_modelSize;
	GLsizei m_instances{0};
	std::size_t m_instancesReservedMemory{64};

public:
	static Mesh CreateSphereMesh(std::size_t subdivisions);
	static Mesh CreatePlaneMesh(std::size_t subdivisions);
	static Mesh CreatePlaneMesh(std::size_t subdivisionsX, std::size_t subdivisionsY);
};

#endif
