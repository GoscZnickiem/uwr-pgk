#ifndef _GZN_PGK_RAWMODEL_
#define _GZN_PGK_RAWMODEL_

#include <cstdint>
#include <vector>

class RawModel {
public:
	RawModel(const std::vector<float>& vertices, const std::vector<int>& indices);
	RawModel(const std::vector<float>& vertices);
	~RawModel();

	RawModel(const RawModel &) = delete;
	RawModel(RawModel &&) = delete;
	RawModel &operator=(const RawModel &) = delete;
	RawModel &operator=(RawModel &&) = delete;

	void bind() const;
	void unbind() const;
	
	uint32_t m_vao = 0;
	uint32_t m_vbo = 0;
	uint32_t m_ebo = 0;
	int vertexCount;

	inline static uint32_t s_currentVAO = 0;

	static RawModel GenerateSphere(std::size_t subdivisions = 3);

private:
	void p_createBuffers(const std::vector<float>& vertices, const std::vector<int>& indices);
};

#endif
