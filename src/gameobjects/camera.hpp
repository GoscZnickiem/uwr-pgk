#ifndef _GZN_PGK_GAMEOBJECTS_CAMERA_
#define _GZN_PGK_GAMEOBJECTS_CAMERA_

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Camera {
public:
	void update(const glm::vec3 playerPos);
	void setup();

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void rotatePitch(float rad);
	void rotateYaw(float rad);

	glm::vec3 position{1.f, 0.f, 1.f};
	glm::vec3 direction{glm::normalize(glm::vec3(1.f, 1.f, 1.f))};
	glm::vec3 up{0.f, 1.f, 0.f};
	float fov{70.f};
	float aspectRatio{1.f};
	float nearPlane{0.1f};
	float farPlane{1000.f};

	bool ortoMode{false};

	bool outsideMode{false};
	float zoom{20.f};
	float targetZoom{20.f};
	float zoomTimer{1.f};
};

#endif
