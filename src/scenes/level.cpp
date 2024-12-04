#include "level.hpp"
#include "../core/appdata.hpp"
#include "../core/input.hpp"
#include "../utility.hpp"
#include <algorithm>
#include <array>

static constexpr float aqWidth = 15.f;
static constexpr float aqHeight = 15.f;
static constexpr float aqDepth = 30.f;

Level::Level(std::size_t bubbleCount)
: aquarium(aqWidth, aqHeight, aqDepth), lvl(bubbleCount) {
	cameras.emplace_back();
	cameras[0].outsideMode = true;
	Input::setMousePosLock(true);

	globalLight.direction = glm::normalize(glm::vec3{1.0f, -1.f, 0.3f});
	globalLight.color = AppData::lightColors[6];
	globalLight.intensity = 0.8f;

	player.transform.position = {-aqDepth + player.transform.scale.x, 0.f, 0.f};
	player.setBounds(aqDepth, aqHeight, aqWidth);

	bubbles.reserve(bubbleCount);
	for(std::size_t i = 0; i < bubbleCount; i++) {
		auto& b = bubbles.emplace_back();
		b.startSize = random(0.5f, 1.5f);
		b.endSize = b.startSize * 1.5f;
		b.startHeight = -aqHeight - b.startSize;
		b.endHeight = aqHeight - 2.f;
		b.speed = random(0.3f, 0.6f);
		float z = random(-aqWidth + b.endSize, aqWidth - b.endSize);
		float x = random(-aqDepth + b.endSize + (z < 4.f && z > -4.f ? 3.f : 0.f), aqDepth - b.endSize);
		float y = random(-aqHeight - 1.f, aqHeight - 3.f);
		b.transform.position = {x, y, z};
	}

	std::size_t lights = std::min(bubbleCount / 10, Shader::LIGHTS_NUM - 1);
	lightBubbles.reserve(lights);
	for(std::size_t i = 0; i < lights; i++) {
		auto& b = lightBubbles.emplace_back(AppData::lightColors[i % 6]);
		b.visual.material = &AppData::Data().materials.light[i % 6];
		b.startSize = 0.6f;
		b.endSize = b.startSize * 1.5f;
		b.startHeight = -aqHeight - b.startSize;
		b.endHeight = aqHeight - 2.f;
		b.speed = random(0.3f, 0.6f);
		float z = random(-aqWidth + b.endSize, aqWidth - b.endSize);
		float x = random(-aqDepth + b.endSize + (z < 5.f && z > -5.f ? 5.f : 0.f), aqDepth - b.endSize);
		float y = random(-aqHeight - 1.f, aqHeight - 3.f);
		b.transform.position = {x, y, z};
	}
}

void Level::update() {
	if(Input::isKeyClicked("TAB")) {
		cameras[0].outsideMode = !cameras[0].outsideMode;
	}
	cameras[0].update(player.transform.position, player.direction);
	if(!cameras[0].outsideMode)
		player.update();
	for(auto& b : bubbles) {
		b.update();
		if(glm::length(b.transform.position - player.transform.position) <= b.transform.scale.x + player.transform.scale.x) {
			AppData::Data().window.close();
		}
	}
	for(auto& b : lightBubbles) {
		b.update();
		if(glm::length(b.transform.position - player.transform.position) <= b.transform.scale.x + player.transform.scale.x) {
			b.collected = true;
			AppData::score += 20;
		}
	}

	if(player.transform.position.x >= aqDepth - player.transform.scale.x || Input::isKeyClicked("ESCAPE")) {
		AppData::score += 100;
		AppData::Data().sceneManager.pushScene<Level>(lvl + 10);
		AppData::Data().sceneManager.switchToNextScene();
		auto [w, h] = AppData::Data().window.getWindowSize();
		AppData::Data().sceneManager.atWindowResize(w, h);
	}
}

void Level::render() {
	cameras[0].setup();

	player.setLight();
	for(auto& b : lightBubbles) b.setLight();
	Shader::SetLightDirectional(globalLight);
	Shader::SetLightsUniform();

	aquarium.fogRender();
	AppData::Data().renderer.writeDepth();

	aquarium.render();
	player.render();
	for(auto& b : bubbles) b.render();
	for(auto& b : lightBubbles) b.render();

	AppData::Data().renderer.render();
}
