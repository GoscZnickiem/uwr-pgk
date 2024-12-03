#include "lightBubble.hpp"

#include "../core/appdata.hpp"
#include <cmath>

LightBubble::LightBubble(glm::vec3 color) {
	visual.mesh = &AppData::Data().models.ball;
	visual.material = &AppData::Data().materials.light;
	visual.transform = &transform;
	light.color = color;
	light.intensity = 1.0f;
}

void LightBubble::update() {
	transform.position.y += speed * AppData::deltaT;
	if(transform.position.y > endHeight) transform.position.y = startHeight;

	float scale = std::lerp(startSize, endSize, (transform.position.y - startHeight) / (endHeight - startHeight));
	transform.scale = {scale, scale, scale};
}

void LightBubble::render() {
	AppData::Data().renderer.addRender(visual);
}

void LightBubble::setLight() {
	light.position = transform.position;
	Shader::SetLightPoint(light);
}
