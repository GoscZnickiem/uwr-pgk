#include "bubble.hpp"

#include "../core/appdata.hpp"
#include <cmath>

Bubble::Bubble() {
	visual.mesh = &AppData::Data().models.ball;
	visual.material = &AppData::Data().materials.bubble;
	visual.transform = &transform;
}

void Bubble::update() {
	transform.position.y += speed * AppData::deltaT;
	if(transform.position.y > endHeight) transform.position.y = startHeight;

	float scale = std::lerp(startSize, endSize, (transform.position.y - startHeight) / (endHeight - startHeight));
	transform.scale = {scale, scale, scale};
}

void Bubble::render() {
	AppData::Data().renderer.addRender(visual);
}
