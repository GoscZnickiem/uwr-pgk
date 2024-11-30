#include "shader.hpp"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <string>

Shader::Shader(const std::string& file) {
	std::ifstream stream;
	stream.open(file);
	if(!stream.is_open()) {
		std::cerr << "Unable to open file " << file << "\n";
		exit(10);
	}

	bool frag = false;
	std::string line;
	std::string vertexSource;
	std::string fragmentSource;
	while(std::getline(stream, line)) {
		if(line.find("#shader") != std::string::npos) {
			frag = line.find(" fragment") != std::string::npos;
			continue;
		}
		(frag ? fragmentSource : vertexSource) += line + "\n";
	}
	stream.close();

	if(vertexSource.empty() || fragmentSource.empty()) {
		std::cerr << "Empty shaders file" << "\n";
		exit(11);
	}

	const uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertex, vertexSource.c_str());
	const uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragment, fragmentSource.c_str());

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	int success = 0;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		int logLength = 0;
		glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &logLength);
		std::string message(static_cast<size_t>(logLength), 0);
		glGetShaderInfoLog(m_ID, logLength, nullptr, message.data());
		std::cerr << "Linking of shaders failed. Error:\n" << message << "\n";
		exit(13);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	const uint32_t camBlockIndex = glGetUniformBlockIndex(m_ID, "Camera");
	glUniformBlockBinding(m_ID, camBlockIndex, 0);
	const uint32_t lightBlockIndex = glGetUniformBlockIndex(m_ID, "Lights");
	glUniformBlockBinding(m_ID, lightBlockIndex, 1);
}

Shader::~Shader() {
	glDeleteProgram(m_ID);
}

void Shader::setUniform(const std::string& name, float v0) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform1f(loc, v0);
}

void Shader::setUniform(const std::string& name, float v0, float v1) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform2f(loc, v0, v1);
}

void Shader::setUniform(const std::string& name, float v0, float v1, float v2) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform3f(loc, v0, v1, v2);
}

void Shader::setUniform(const std::string& name, float v0, float v1, float v2, float v3) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform4f(loc, v0, v1, v2, v3);
}

void Shader::setUniform(const std::string& name, int v0) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform1i(loc, v0);
}

void Shader::setUniform(const std::string& name, int v0, int v1) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform2i(loc, v0, v1);
}

void Shader::setUniform(const std::string& name, int v0, int v1, int v2) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform3i(loc, v0, v1, v2);
}

void Shader::setUniform(const std::string& name, int v0, int v1, int v2, int v3) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform4i(loc, v0, v1, v2, v3);
}

void Shader::setUniform(const std::string& name, unsigned int v0) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform1ui(loc, v0);
}

void Shader::setUniform(const std::string& name, unsigned int v0, unsigned int v1) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform2ui(loc, v0, v1);
}

void Shader::setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform3ui(loc, v0, v1, v2);
}

void Shader::setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform4ui(loc, v0, v1, v2, v3);
}

void Shader::setUniform(const std::string& name,const glm::vec2& v) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform2fv(loc, 1, &v[0]);
}

void Shader::setUniform(const std::string& name,const glm::vec3& v) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform3fv(loc, 1, &v[0]);
}

void Shader::setUniform(const std::string& name,const glm::vec4& v) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniform4fv(loc, 1, &v[0]);
}

void Shader::setUniform(const std::string& name,const glm::mat2& v) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::setUniform(const std::string& name,const glm::mat3& v) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::setUniform(const std::string& name,const glm::mat4& v) const {
	const int32_t loc = glGetUniformLocation(m_ID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::SetCameraUniform(const Camera& camera) {
	glBindBuffer(GL_UNIFORM_BUFFER, s_cameraUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getProjectionMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(camera.position));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shader::SetLightDirectional(const Light& light) {
	s_lights[0] = light;
}

void Shader::SetLightPoint(const Light& light) {
	if(s_currentLightIndex > LIGHTS_NUM) return;
	s_lights[s_currentLightIndex++] = light;
}

void Shader::SetLightsUniform() {
	glBindBuffer(GL_UNIFORM_BUFFER, s_lightsUBO);
	s_lights.fill({{glm::vec3{1.f, 1.f, 1.f}},glm::vec3{1.f, 1.f, 1.f}, 1.f});
	// glBufferSubData(GL_UNIFORM_BUFFER, 0, (1 + LIGHTS_NUM) * sizeof(glm::vec3), glm::value_ptr(s_lights.data()->direction));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(s_lights.data()->direction));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	std::cout << glm::to_string(s_lights[0].direction) << "\n";
	s_currentLightIndex = 0;
	// s_lights.fill({{},{},0.f});
}

void Shader::bind() const {
	glUseProgram(m_ID);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::CreateCameraUBO() {
	glGenBuffers(1, &s_cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, s_cameraUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_cameraUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shader::CreateLightUBO() {
	glGenBuffers(1, &s_lightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, s_lightsUBO);
	// glBufferData(GL_UNIFORM_BUFFER, (1 + LIGHTS_NUM) * sizeof(Light), nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, s_lightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shader::compileShader(uint32_t shader, const char* source) {
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		int logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::string message(static_cast<size_t>(logLength), 0);
		glGetShaderInfoLog(shader, logLength, &logLength, message.data());
		std::cerr << "Compilaton of a shader failed. Error:\n" << message << "\n";
		exit(12);
	}
}
