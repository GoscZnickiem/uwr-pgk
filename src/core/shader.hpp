#ifndef _GZN_PGK_SHADER_
#define _GZN_PGK_SHADER_

#include <cstdint>
#include <list>
#include <string>

class Shader {
public:
	explicit Shader(const std::string& file);
	~Shader();
	Shader(const Shader &) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&) = delete;

	void setUniform(const std::string& name, float v0) const;
	void setUniform(const std::string& name, float v0, float v1) const;
	void setUniform(const std::string& name, float v0, float v1, float v2) const;
	void setUniform(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setUniform(const std::string& name, int v0) const;
	void setUniform(const std::string& name, int v0, int v1) const;
	void setUniform(const std::string& name, int v0, int v1, int v2) const;
	void setUniform(const std::string& name, int v0, int v1, int v2, int v3) const;
	void setUniform(const std::string& name, unsigned int v0) const;
	void setUniform(const std::string& name, unsigned int v0, unsigned int v1) const;
	void setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2) const;
	void setUniform(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;

	template<typename ... Args>
	static void setGlobalUniform(const std::string& name, Args... args) {
		for(auto* shader : s_shaders) {
			shader->bind();
			shader->setUniform("global_" + name, args...);
		}
	}

	void bind() const;
	void unbind() const;

private:
	uint32_t m_ID = 0;
	std::list<Shader*>::iterator m_thisIt;

	inline static uint32_t s_currentShader = 0;
	inline static std::list<Shader*> s_shaders;

	void compileShader(uint32_t shader, const char* source);
};

#endif
