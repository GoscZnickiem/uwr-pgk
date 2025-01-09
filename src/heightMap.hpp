#ifndef _GZN_PGK_HEIGHTMAP_
#define _GZN_PGK_HEIGHTMAP_

#include "core/window.hpp"
#include <array>
#include <string>

struct HeightMap {
	inline static constexpr std::size_t side = 1201;
	inline static constexpr std::size_t size = side * side;

	static inline constexpr std::size_t LODS = 10;
	static inline constexpr std::array<GLuint, 10> LOD_DIVS = {1, 2, 4, 6, 10, 15, 20, 30, 60, 100};
	static inline std::array<GLuint, 10> ebo;
	static inline std::array<GLint, 10> eboSize;
	static inline std::string readDirectory = "./";

	int latitude;
	int longitude;
	bool loaded{false};
	GLuint vao;
	GLuint vbo;

	HeightMap(int lat, int lon);
	~HeightMap();

	void load();
	void unload();
	void render(std::size_t lod);

	static void setReadDirectory(std::string directory);
	static void CreateEbos();
	static void DeleteEbos();
};

#endif
