#ifndef _GZN_PGK_HEIGHTMAP_
#define _GZN_PGK_HEIGHTMAP_

#include "core/window.hpp"
#include <array>
#include <glm/ext/vector_float3.hpp>
#include <string>

struct HeightMap {
	static inline constexpr std::size_t side = 1201;
	static inline constexpr std::size_t size = side * side;

	static inline constexpr std::size_t LODS = 9;
	static inline constexpr std::array<GLuint, LODS> LOD_DIVS = {1, 2, 4, 6, 10, 15, 20, 30, 60};
	static inline std::array<GLuint, LODS> ebo;
	static inline std::array<GLint, LODS> eboSize;
	static inline std::size_t lod = LODS / 2;

	enum class State {
		LOADED, LOADING, REQUESTED, MARKED, UNLOADED, UNAVAIBLE 
	} state;
	GLuint vao;
	GLuint vbo;
	std::string sourceFile;
	glm::vec3 worldPos;

	HeightMap();
	~HeightMap();

	void requestLoad();
	void requestUnload();
	void load();
	void unload();
	void render();

	static void CreateEbos();
	static void DeleteEbos();

	static void LoadRequestedMaps();
	static void UnloadRequestedMaps();
	static void GenerateRequestedBuffers();
};

#endif
