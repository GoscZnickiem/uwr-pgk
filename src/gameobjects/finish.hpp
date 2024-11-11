#ifndef _GZN_PGK_GAMEOBJECTS_FINISH_
#define _GZN_PGK_GAMEOBJECTS_FINISH_

#include "../model.hpp"

class Finish {
public:
	explicit Finish();

	Transform transform;

	void update();
	void render();

private:
	Model m_vis;
};

#endif
