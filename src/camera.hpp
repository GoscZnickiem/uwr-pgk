#ifndef _GZN_PGK_GAMEOBJECTS_CAMERA_
#define _GZN_PGK_GAMEOBJECTS_CAMERA_

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Camera {
public:
	void update(const glm::vec3 playerPos, const glm::vec3 playerDir);
	void setup();

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void rotatePitch(float rad);
	void rotateYaw(float rad);

	void updateResolution(int w, int h);

	glm::vec3 position{1.f, 0.f, 1.f};
	glm::vec3 direction{glm::normalize(glm::vec3(0.f, -1.f, -3.f))};
	glm::vec3 up{0.f, 1.f, 0.f};
	float fov{70.f};
	float aspectRatio{1.f};
	float nearPlane{0.001f};
	float farPlane{1000.f};

	bool outsideMode{false};
	float minFov{50.f};
	float maxFov{120.f};
	float targetFov{1.f};
	float fovTimer{1.f};
	glm::vec3 lookat{0.f, 0.f, 0.f};

	glm::vec3 dirBuffer{direction};
};

#endif
