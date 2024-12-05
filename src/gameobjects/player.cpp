#include "player.hpp"
#include "../core/appdata.hpp"
#include "../core/input.hpp"
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>

Player::Player() {
	visual.mesh = &AppData::Data().models.ball;
	visual.material = &AppData::Data().materials.player;
	visual.transform = &transform;

	transform.scale = {0.5f, 0.5f, 0.5f};
	direction = {1,0,0};

	light.color = {1, 0, 0};
	light.intensity = 1.f;
}

void Player::update() {
	const float speed = 6.0f * AppData::deltaT;

	if(Input::isKeyPressed("W")) {
		transform.position += speed * direction;
	}
	if(Input::isKeyPressed("S")) {
		transform.position -= speed * direction;
	}
	if(Input::isKeyPressed("A")) {
		transform.position -= speed * glm::normalize(glm::cross(direction, {0,1,0}));
	}
	if(Input::isKeyPressed("D")) {
		transform.position += speed * glm::normalize(glm::cross(direction, {0,1,0}));
	}
	if(Input::isKeyPressed("SPACE")) {
		transform.position += speed * glm::vec3{0,1,0};
	}
	if(Input::isKeyPressed("SHIFT")) {
		transform.position -= speed * glm::vec3{0,1,0};
	}

	transform.position.x = std::clamp(transform.position.x, -aquariumBounds.x, aquariumBounds.x + 5.f);
	transform.position.y = std::clamp(transform.position.y, -aquariumBounds.y, aquariumBounds.y);
	transform.position.z = std::clamp(transform.position.z, -aquariumBounds.z, aquariumBounds.z);


	constexpr float sensitivity = 0.01f;
	constexpr float rotateSpeed = 1.f * AppData::deltaT;
	direction = glm::normalize(direction);

	if(Input::isKeyPressed("UP")) rotatePitch(rotateSpeed);
	if(Input::isKeyPressed("DOWN")) rotatePitch(-rotateSpeed);
	if(Input::isKeyPressed("RIGHT")) rotateYaw(-rotateSpeed);
	if(Input::isKeyPressed("LEFT")) rotateYaw(rotateSpeed);

	auto[mouseX, mouseY] = Input::getMousePos();
	if(mouseX != 0) rotateYaw(-mouseX * sensitivity);
	if(mouseY != 0) rotatePitch(-mouseY * sensitivity);
}

void Player::render() {
	AppData::Data().renderer.addRender(visual);
}

void Player::setLight() {
	light.position = transform.position;
	Shader::SetLightPoint(light);
}

void Player::rotatePitch(float rad) {
	constexpr float limit = 0.98f;
	if((rad > 0 && direction.y >= limit) || (rad < 0 && direction.y <= -limit)) return;
	direction = glm::rotate(glm::mat4(1.f), rad, glm::normalize(glm::cross(direction, {0,1,0}))) * glm::vec4(direction, 1.f);
}

void Player::rotateYaw(float rad) {
	direction = glm::rotate(glm::mat4(1.f), rad, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(direction, 1.f);
}

void Player::setBounds(float width, float height, float depth) {
	aquariumBounds.x = width - transform.scale.x;
	aquariumBounds.y = height - transform.scale.y;
	aquariumBounds.z = depth - transform.scale.z;
}
