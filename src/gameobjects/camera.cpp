#include "camera.hpp"
#include "../core/shader.hpp"
#include "../core/input.hpp"
#include "../appdata.hpp"
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
		position = direction * (-zoom);
		if (Input::getScroll() >= 1) {
			targetZoom *= 0.8f;
			zoomTimer = 0;
		}
		if (Input::getScroll() <= -1) {
			targetZoom *= 1.25f;
			zoomTimer = 0;
		}
		targetZoom = std::clamp(targetZoom, 8.0f, 30.f);
		zoomTimer += AppData::deltaT * 2.0f; 
		zoomTimer = std::min(zoomTimer, 1.f);
		zoom = std::lerp(zoom, targetZoom, zoomTimer);
	} else {
		position = playerPos;
	}

}

void Camera::setup() {
	glViewport(viewPosP.first, viewPosP.second, viewSizeP.first, viewSizeP.second);
	Shader::SetCameraUniform(getViewMatrix(), getProjectionMatrix());
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(position, position + direction, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
	if(outsideMode && ortoMode)
		return glm::ortho(-zoom/2 * aspectRatio, zoom/2 * aspectRatio, -zoom/2, zoom/2, nearPlane, farPlane);
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
	viewSizeP = { static_cast<int>(w * viewSize.first), static_cast<int>(h * viewSize.first) };
	viewPosP = { static_cast<int>(w * viewPos.first), static_cast<int>(h * viewPos.first) };
}
