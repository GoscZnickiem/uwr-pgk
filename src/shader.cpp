#include "shader.hpp"
#include <GL/glew.h>
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

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	int success = 0;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		int logLength = 0;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);
		std::string message(static_cast<size_t>(logLength), 0);
		glGetShaderInfoLog(ID, logLength, nullptr, message.data());
		std::cerr << "Linking of shaders failed. Error:\n" << message << "\n";
		exit(13);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

// void Shader::setUniform(const std::string& name, float v0) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform1f(loc, v0);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, float v0, float v1) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform2f(loc, v0, v1);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, float v0, float v1, float v2) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform3f(loc, v0, v1, v2);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, float v0, float v1, float v2, float v3) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform4f(loc, v0, v1, v2, v3);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, int v0) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform1i(loc, v0);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, int v0, int v1) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform2i(loc, v0, v1);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, int v0, int v1, int v2) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform3i(loc, v0, v1, v2);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, int v0, int v1, int v2, int v3) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform4i(loc, v0, v1, v2, v3);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, unsigned int v0) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform1ui(loc, v0);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, unsigned int v0, unsigned int v1) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform2ui(loc, v0, v1);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform3ui(loc, v0, v1, v2);
// 	glUseProgram(currentShader);
// }
//
// void Shader::setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const {
// 	glUseProgram(ID);
// 	const int32_t loc = glGetUniformLocation(ID, name.c_str());
// 	glUniform4ui(loc, v0, v1, v2, v3);
// 	glUseProgram(currentShader);
// }

void Shader::bind() const {
	glUseProgram(ID);
	currentShader = ID;
}

void Shader::unbind() const {
	glUseProgram(0);
	currentShader = 0;
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
