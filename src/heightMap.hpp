#ifndef _GZN_PGK_HEIGHTMAP_
#define _GZN_PGK_HEIGHTMAP_

#include "core/window.hpp"
#include <array>
#include <string>

struct HeightMap {
	static inline constexpr std::size_t side = 1201;
	static inline constexpr std::size_t size = side * side;

	static inline constexpr std::size_t LODS = 10;
	static inline constexpr std::array<GLuint, 10> LOD_DIVS = {1, 2, 4, 6, 10, 15, 20, 30, 60, 100};
	static inline std::array<GLuint, 10> ebo;
	static inline std::array<GLint, 10> eboSize;
	static inline std::string readDirectory = "./";

	enum class State {
		LOADED, LOADING, UNLOADED, UNAVAIBLE
	} state{State::UNAVAIBLE};
	GLuint vao;
	GLuint vbo;

	~HeightMap();

	void load(int latitude, int longitude);
	void unload(int latitude, int longitude);
	void unload();
	void render(std::size_t lod);

	static void SetReadDirectory(std::string directory);
	static void CreateEbos();
	static void DeleteEbos();

	static void GenerateRequestedBuffers();
};

#endif
