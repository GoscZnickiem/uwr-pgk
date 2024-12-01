#ifndef _GZN_PGK_SCENES_LEVEL_
#define _GZN_PGK_SCENES_LEVEL_

#include "../core/scene.hpp"
#include "../graphics/light.hpp"

class Level : public Scene {
	Level(std::size_t bubbleCount);

	void update() override;
	void render() override;

private:
	Light globalLight;
};

#endif
