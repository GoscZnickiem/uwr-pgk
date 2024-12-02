#ifndef _GZN_PGK_SCENES_LEVEL_
#define _GZN_PGK_SCENES_LEVEL_

#include "../core/scene.hpp"
#include "../graphics/light.hpp"
#include "../gameobjects/aquarium.hpp"
#include "../gameobjects/bubble.hpp"
#include <vector>

class Level : public Scene {
public:
	Level(std::size_t bubbleCount);

	void update() override;
	void render() override;

private:
	Light globalLight;

	Aquarium aquarium;

	std::vector<Bubble> bubbles;
};

#endif
