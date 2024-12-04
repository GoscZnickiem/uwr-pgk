#include "lightBubble.hpp"

#include "../core/appdata.hpp"
#include <cmath>

LightBubble::LightBubble(glm::vec3 color) {
	visual.mesh = &AppData::Data().models.ball;
	visual.material = &AppData::Data().materials.light[8];
	visual.transform = &transform;
	light.color = color;
	light.intensity = 1.5f;
}

void LightBubble::update() {
	if(collected) transform.position.y = -20000.f;
	transform.position.y += speed * AppData::deltaT;
	if(transform.position.y > endHeight) transform.position.y = startHeight;

	float scale = std::lerp(startSize, endSize, (transform.position.y - startHeight) / (endHeight - startHeight));
	transform.scale = {scale, scale, scale};
}

void LightBubble::render() {
	if(collected) return;
	AppData::Data().renderer.addRender(visual);
}

void LightBubble::setLight() {
	if(collected) return;
	light.position = transform.position;
	Shader::SetLightPoint(light);
}
