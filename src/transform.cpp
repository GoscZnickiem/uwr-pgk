#include "transform.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

glm::mat4 Transform::getMatrix() const {
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);

	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
	rot = glm::rotate(rot, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
	rot = glm::rotate(rot, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);

	return trans * rot * scl;
}
