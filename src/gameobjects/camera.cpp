#include "camera.hpp"
#include "../core/appdata.hpp"
#include "../core/shader.hpp"
#include "../core/input.hpp"
#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

void Camera::update(const glm::vec3 playerPos) {
	constexpr float sensitivity = 0.01f;
	constexpr float speed = 0.01f;

	if(Input::isKeyPressed("UP")) rotatePitch(speed);
	if(Input::isKeyPressed("DOWN")) rotatePitch(-speed);
	if(Input::isKeyPressed("RIGHT")) rotateYaw(-speed);
	if(Input::isKeyPressed("LEFT")) rotateYaw(speed);

	auto[mouseX, mouseY] = Input::getMousePos();
	if(mouseX != 0) rotateYaw(-mouseX * sensitivity);
	if(mouseY != 0) rotatePitch(-mouseY * sensitivity);
	
	if(outsideMode) {
		position = -4.f * direction + lookat;
		if (Input::getScroll() >= 1 || Input::isKeyClicked("+")) {
			targetFov -= 10.f;
			fovTimer = 0;
		}
		if (Input::getScroll() <= -1 || Input::isKeyClicked("-")) {
			targetFov += 10.f;
			fovTimer = 0;
		}
		targetFov = std::clamp(targetFov, minFov, maxFov);
		fovTimer += AppData::deltaT * 4.0f; 
		fovTimer = std::min(fovTimer, 1.f);
		fov = std::lerp(fov, targetFov, fovTimer);
	} else {
		position = playerPos;
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

void Camera::rotatePitch(float rad) {
	constexpr float limit = 0.98f;
	if((rad > 0 && direction.y >= limit) || (rad < 0 && direction.y <= -limit)) return;
	direction = glm::rotate(glm::mat4(1.f), rad, glm::normalize(glm::cross(direction, up))) * glm::vec4(direction, 1.f);
}

void Camera::rotateYaw(float rad) {
	direction = glm::rotate(glm::mat4(1.f), rad, glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(direction, 1.f);
}

void Camera::updateResolution(int w, int h) {
	aspectRatio = static_cast<float>(w) / static_cast<float>(h);
}
