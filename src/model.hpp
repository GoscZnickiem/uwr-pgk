#ifndef _GZN_PGK_MODEL_
#define _GZN_PGK_MODEL_

#include "shader.hpp"
#include <cstdint>
#include <vector>

class Model {
public:
	Model(const std::vector<float>& vertices, const std::vector<int>& indices, const Shader& sh);
	Model(const std::vector<float>& vertices, const Shader& sh);
	~Model();

	Model(const Model &) = delete;
	Model(Model &&) = delete;
	Model &operator=(const Model &) = delete;
	Model &operator=(Model &&) = delete;

	void bind() const;
	void unbind() const;
	void render() const;
	
	const Shader* shader;

private:
	uint32_t m_vao = 0;
	uint32_t m_vbo = 0;
	uint32_t m_ebo = 0;
	int m_vertices;

	inline static uint32_t s_currentVAO = 0;

	void p_createBuffers(const std::vector<float>& vertices, const std::vector<int>& indices);
};

#endif