#include "level.hpp"
#include "../core/appdata.hpp"
#include "../core/input.hpp"
#include "../utility.hpp"
#include <algorithm>

static constexpr float aqHeight = 15.f;
static constexpr float aqWidth = 15.f;
static constexpr float aqDepth = 30.f;

Level::Level(std::size_t bubbleCount)
: aquarium(aqWidth, aqHeight, aqDepth) {
	cameras.emplace_back();
	cameras[0].outsideMode = true;
	Input::setMousePosLock(true);

	globalLight.direction = glm::normalize(glm::vec3{0.4f, -1.f, 0.3f});
	globalLight.color = {1.05f, 1.0f, 0.95f};
	globalLight.intensity = 1.0f;

	bubbles.reserve(bubbleCount);
	for(std::size_t i = 0; i < bubbleCount; i++) {
		auto& b = bubbles.emplace_back();
		b.startHeight = -aqHeight;
		b.endHeight = aqHeight - 1.f;
		b.startSize = random(0.5f, 1.5f);
		b.endSize = b.startSize * 1.5f;
		b.speed = random(1.3f, 1.6f);
		b.transform.position = {random(-aqDepth + b.startSize, aqDepth - b.startSize), random(-aqHeight - 1.f, aqHeight - 2.f), random(-aqWidth + b.startSize, aqWidth - b.startSize)};
	}

	std::size_t lights = std::min(bubbleCount / 2, Shader::LIGHTS_NUM);
	lightBubbles.reserve(lights);
	for(std::size_t i = 0; i < lights; i++) {
		auto& b = lightBubbles.emplace_back(glm::vec3{random(0,1), random(0,1), random(0,1)});
		b.startHeight = -aqHeight;
		b.endHeight = aqHeight - 1.f;
		b.startSize = 0.6f;
		b.endSize = b.startSize * 1.5f;
		b.speed = random(1.3f, 1.6f);
		b.light.intensity = random(5.0f, 7.f);
		b.transform.position = {random(-aqDepth + b.startSize, aqDepth - b.startSize), random(-aqHeight - 1.f, aqHeight - 2.f), random(-aqWidth + b.startSize, aqWidth - b.startSize)};
	}
}

void Level::update() {
	cameras[0].update({0,0,0}, {1,0,0});
	for(auto& b : bubbles) b.update();
	for(auto& b : lightBubbles) b.update();
}

void Level::render() {
	cameras[0].setup();

	for(auto& b : lightBubbles) b.setLight();
	Shader::SetLightDirectional(globalLight);
	Shader::SetLightsUniform();

	aquarium.fogRender();
	AppData::Data().renderer.writeDepth();

	aquarium.render();
	for(auto& b : bubbles) b.render();
	for(auto& b : lightBubbles) b.render();

	AppData::Data().renderer.render();
}
