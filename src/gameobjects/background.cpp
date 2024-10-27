#include "background.hpp"
#include "../appdata.hpp"

#include <GL/glew.h>

BackGround::BackGround()
: m_shader(&AppData::data().shaderBackground) {
	glGenVertexArrays(1, &m_vao);
}

BackGround::~BackGround() {
	glDeleteVertexArrays(1, &m_vao);
}

void BackGround::render(float time) {
	m_shader->bind();
	glBindVertexArray(m_vao);

	m_shader->setUniform("time", time);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	m_shader->unbind();
}
