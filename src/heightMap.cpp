#include "heightMap.hpp"
#include <array>

void HeightMap::createBuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(data)), data.data(), GL_STATIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_SHORT, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HeightMap::deleteBuffer() {
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void HeightMap::render(std::size_t lod) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[lod]);
	glDrawElements(GL_TRIANGLES, eboSize[lod], GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
}

template<std::size_t S>
struct Lod {
	static constexpr std::size_t Side = ((HeightMap::side - 1) / S);
	static constexpr std::size_t N = Side * Side * 6;
	static std::array<GLuint, N>* generateIndices() {
		auto* arr = new std::array<GLuint, N>();
		for(GLuint squareY = 0; squareY < Side; squareY++) {
			for(GLuint squareX = 0; squareX < Side; squareX++) {
				const GLuint i = static_cast<GLuint>(squareY * Side + squareX);
				(*arr)[i*6]	= static_cast<GLuint>((squareY * HeightMap::side + squareX) * S);
				(*arr)[i*6+1]	= static_cast<GLuint>((squareY * HeightMap::side + squareX + 1) * S);
				(*arr)[i*6+2]	= static_cast<GLuint>(((squareY + 1) * HeightMap::side + squareX) * S);
				(*arr)[i*6+3]	= static_cast<GLuint>((squareY * HeightMap::side + squareX + 1) * S);
				(*arr)[i*6+4]	= static_cast<GLuint>((squareY * HeightMap::side + squareX + 1) * S);
				(*arr)[i*6+5]	= static_cast<GLuint>(((squareY + 1) * HeightMap::side + squareX) * S);
			}
		}
		return arr;
	}
};

void HeightMap::CreateEbos() {
	auto* indices = Lod<1>::generateIndices();
	glGenBuffers(1, &ebo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices->size() * sizeof(int)), indices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	eboSize[0] = indices->size();
	delete indices;
} 

void HeightMap::DeleteEbos() {
    glDeleteBuffers(1, &ebo[0]);
}
