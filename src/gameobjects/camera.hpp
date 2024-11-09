#ifndef _GZN_PGK_GAMEOBJECTS_CAMERA_
#define _GZN_PGK_GAMEOBJECTS_CAMERA_

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Camera {
public:
	void update();
	void setup();

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	glm::vec3 position{1.f, 0.f, 1.f};
	glm::vec3 direction{glm::normalize(glm::vec3(-1.f, -0.3f, -1.f))};
	glm::vec3 up{0.f, 1.f, 0.f};
	float fov{70.f};
	float aspectRatio{1.f};
	float nearPlane{0.1f};
	float farPlane{1000.f};
};

#endif
