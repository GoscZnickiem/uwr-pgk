#include "heightMap.hpp"
#include "core/appdata.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <array>

inline static constexpr uint16_t swapBytes(uint16_t v) {
	return (v >> 8) | (v << 8);
}

HeightMap::~HeightMap() {
	if(!loaded) unload();
}

void HeightMap::load(int latitude, int longitude) {
	if(!exists || loaded) return;

	const char latSign = latitude < 0 ? 'S' : 'N';
	const char lonSign = longitude < 0 ? 'W' : 'E';

	std::stringstream ss;
	ss  << readDirectory 
		<< latSign 
		<< std::setw(2) << std::setfill('0') << std::abs(latitude)
		<< lonSign
		<< std::setw(3) << std::setfill('0') << std::abs(longitude)
		<< ".hgt";

	std::ifstream file(ss.str(), std::ios::binary);
	if(!file) {
		std::cerr << "Error: Failed to open the file \"" << ss.str() << "\"\n";
		exit(3);
	}

	auto data = std::make_unique<uint16_t[]>(size);

	file.read(reinterpret_cast<char*>(data.get()), size * 2);
	if (!file) {
		std::cerr << "Error: Failed to read the file \"" << ss.str() << "\". Bytes read: " << file.gcount() << "\n";
		exit(3);
	}
	file.close();

	for (std::size_t i = 0; i < size; i++)
		data[i] = swapBytes(data[i]);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size * sizeof(uint16_t)), data.get(), GL_STATIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_SHORT, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	loaded = true;

	std::cout << "loaded " << ss.str() << "\n";
}

void HeightMap::unload() {
	if(!exists || !loaded) return;

	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

	loaded = false;
}

void HeightMap::unload(int latitude, int longitude) {
	if(!exists || !loaded) return;

	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

	loaded = false;

	const char latSign = latitude < 0 ? 'S' : 'N';
	const char lonSign = longitude < 0 ? 'W' : 'E';

	std::stringstream ss;
	ss  << readDirectory 
		<< latSign 
		<< std::setw(2) << std::setfill('0') << std::abs(latitude)
		<< lonSign
		<< std::setw(3) << std::setfill('0') << std::abs(longitude)
		<< ".hgt";
	std::cout << "unloaded " << ss.str() << "\n";
}

void HeightMap::render(std::size_t lod) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[lod]);
	glDrawElements(GL_TRIANGLES, eboSize[lod], GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
}




void HeightMap::setReadDirectory(std::string directory) {
    if (!directory.empty() && directory.back() != '/')
        directory += '/';
    readDirectory = std::move(directory);
}

template<GLuint S>
struct Lod {
	static constexpr GLuint D = static_cast<GLuint>(HeightMap::side);
	static constexpr GLuint Side = (D - 1) / S;
	static constexpr std::size_t N = Side * Side * 6;
	static std::array<GLuint, N>* generateIndices() {
		auto* arr = new std::array<GLuint, N>();
		for(GLuint y = 0; y < Side; y++) {
			for(GLuint x = 0; x < Side; x++) {
				const GLuint i = y * Side + x;
				(*arr)[i*6]		= ((y + 0) * D + x + 0) * S;
				(*arr)[i*6+1]	= ((y + 0) * D + x + 1) * S;
				(*arr)[i*6+2]	= ((y + 1) * D + x + 0) * S;
				(*arr)[i*6+3]	= ((y + 1) * D + x + 0) * S;
				(*arr)[i*6+4]	= ((y + 1) * D + x + 1) * S;
				(*arr)[i*6+5]	= ((y + 0) * D + x + 1) * S;
			}
		}
		return arr;
	}
};

template<GLuint S>
void createEbo() {
	auto* indices = Lod<HeightMap::LOD_DIVS[S]>::generateIndices();
	glGenBuffers(1, &HeightMap::ebo[S]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HeightMap::ebo[S]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices->size() * sizeof(GLuint)), indices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	HeightMap::eboSize[S] = static_cast<GLint>(indices->size());
	delete indices;
}

void HeightMap::CreateEbos() {
	createEbo<0>();
	createEbo<1>();
	createEbo<2>();
	createEbo<3>();
	createEbo<4>();
	createEbo<5>();
	createEbo<6>();
	createEbo<7>();
	createEbo<8>();
	createEbo<9>();
} 

void HeightMap::DeleteEbos() {
    glDeleteBuffers(10, &ebo[0]);
}

