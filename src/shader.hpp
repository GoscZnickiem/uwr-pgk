#ifndef _GZN_PGK_SHADER_
#define _GZN_PGK_SHADER_

#include <cstdint>
#include <string>

class Shader {
public:
	explicit Shader(const std::string& file);
	~Shader();
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&) = delete;

	// I should really rewrite this:
	// void setUniform(const std::string& name, float v0) const;
	// void setUniform(const std::string& name, float v0, float v1) const;
	// void setUniform(const std::string& name, float v0, float v1, float v2) const;
	// void setUniform(const std::string& name, float v0, float v1, float v2, float v3) const;
	// void setUniform(const std::string& name, int v0) const;
	// void setUniform(const std::string& name, int v0, int v1) const;
	// void setUniform(const std::string& name, int v0, int v1, int v2) const;
	// void setUniform(const std::string& name, int v0, int v1, int v2, int v3) const;
	// void setUniform(const std::string& name, unsigned int v0) const;
	// void setUniform(const std::string& name, unsigned int v0, unsigned int v1) const;
	// void setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2) const;
	// void setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;

	void bind() const;
	void unbind() const;

private:
	uint32_t ID = 0;
	inline static uint32_t currentShader = 0;
	void compileShader(uint32_t shader, const char* source);
};

#endif
