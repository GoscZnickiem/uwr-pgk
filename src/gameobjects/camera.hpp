#ifndef _GZN_PGK_GAMEOBJECTS_CAMERA_
#define _GZN_PGK_GAMEOBJECTS_CAMERA_

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <utility>

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
	float nearPlane{0.001f};
	float farPlane{1000.f};

	bool ortoMode{false};

	bool outsideMode{false};
	float zoom{20.f};
	float targetZoom{20.f};
	float zoomTimer{1.f};

	void updateResolution(int w, int h);

	std::pair<double, double> viewSize{1., 1.};
	std::pair<double, double> viewPos{0., 0.};
	std::pair<int, int> viewSizeP{1, 1};
	std::pair<int, int> viewPosP{0, 0};
};

#endif
