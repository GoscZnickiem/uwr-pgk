#ifndef _GZN_PGK_DRAWABLE
#define _GZN_PGK_DRAWABLE

#include "core/shader.hpp"
#include "rawModel.hpp"
#include "transform.hpp"

class Model {
public:
	Model(Transform& transform, const RawModel& model, const Shader& shader);

	void render() const;

private:
	Transform* m_transform;
	const RawModel* m_model;
	const Shader* m_shader;
};

#endif
