#ifndef _GZN_PGK_HEIGHTMAP_
#define _GZN_PGK_HEIGHTMAP_

#include "core/window.hpp"
#include <array>
#include <cstdint>

struct HeightMap {
	inline static constexpr std::size_t side = 1201;
	inline static constexpr std::size_t size = side * side;

	std::array<uint16_t, size> data;
	int latitude;
	int longitude;

	GLuint vao;
	GLuint vbo;
	static inline std::array<GLuint, 10> ebo;
	static inline std::array<GLint, 10> eboSize;

	void createBuffer();
	void deleteBuffer();
	void render(std::size_t lod);

	static void CreateEbos();
	static void DeleteEbos();
};

#endif
