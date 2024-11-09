#include "camera.hpp"
#include "../core/shader.hpp"
#include "../core/input.hpp"
#include "../appdata.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

void Camera::update() {
	constexpr float speed = 2.0f * AppData::deltaT;
	constexpr float sensitivity = 0.01f;

	if(Input::isKeyPressed("UP")) {
		position += direction * speed;
	}
	if(Input::isKeyPressed("DOWN")) {
		position -= direction * speed;
	}
	if(Input::isKeyPressed("RIGHT")) {
		position += glm::normalize(glm::cross(direction, up)) * speed;
	}
	if(Input::isKeyPressed("LEFT")) {
		position -= glm::normalize(glm::cross(direction, up)) * speed;
	}

	if(!Input::isMouseLocked()) return;
	auto[mouseX, mouseY] = Input::getMousePos();
	if(mouseX != 0) {
		direction = glm::rotate(glm::mat4(1.f), -mouseX * sensitivity, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(direction, 1.f);
	}
	if(mouseY != 0) {
		direction = glm::rotate(glm::mat4(1.f), -mouseY * sensitivity, glm::normalize(glm::cross(direction, up))) * glm::vec4(direction, 1.f);
	}

}

void Camera::setup() {
	Shader::SetCameraUniform(getViewMatrix(), getProjectionMatrix());
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(position, position + direction, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
