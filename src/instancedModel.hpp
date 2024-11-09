#ifndef _GZN_PGK_INSTANCEDMODEL_
#define _GZN_PGK_INSTANCEDMODEL_

#include "core/shader.hpp"
#include "rawModel.hpp"

class InstancedModel {
public:
	InstancedModel(const std::vector<float>& vertices, const std::vector<int>& indices, const Shader& shader);
	InstancedModel(const std::vector<float>& vertices, const Shader& shader);
	~InstancedModel();

	void instantiate(const std::vector<glm::mat4>& transforms);
	void render() const;

private:
	const RawModel m_model;
	const Shader* m_shader;
	uint32_t m_vbo;
	int m_instances{0};
};

#endif
