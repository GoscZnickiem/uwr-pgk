#ifndef _GZN_PGK_DRAWABLE
#define _GZN_PGK_DRAWABLE

#include "model.hpp"
#include "transform.hpp"

class Drawable {
public:
	Drawable(Transform& transform, const Model& model);

	void render() const;

private:
	Transform* m_transform;
	const Model* m_model;
};

#endif
