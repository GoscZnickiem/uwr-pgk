#ifndef _GZN_PGK_DRAWABLE
#define _GZN_PGK_DRAWABLE

#include "model.hpp"
class Drawable {
public:
	explicit Drawable(const Model& model);

	void render() const;

	float x;
	float y;
	float angle;
	float xScale;
	float yScale;

private:
	const Model* m_model;
};

#endif
