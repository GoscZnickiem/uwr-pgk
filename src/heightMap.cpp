#include "heightMap.hpp"
#include "core/appdata.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <array>
#include <thread>

inline static constexpr uint16_t swapBytes(uint16_t v) {
	return (v >> 8) | (v << 8);
}

struct BufferRequest {
	HeightMap* target;
	std::unique_ptr<uint16_t[]> data;
};

static std::queue<BufferRequest> bufferRequests;
static std::mutex requestsMutex;



HeightMap::HeightMap() : state(State::UNAVAIBLE) {}

HeightMap::~HeightMap() {
	GenerateRequestedBuffers();
	if(state == State::LOADED) unload();
}

void HeightMap::load() {
	if(state != State::UNLOADED) return;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size * sizeof(uint16_t)), nullptr, GL_STATIC_DRAW);
	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY) {
		std::cerr << "Error: insufficient memory to load " << sourceFile << "\n";
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		return;
	}
	glVertexAttribIPointer(0, 1, GL_SHORT, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	state = State::LOADING;
	std::thread([this]() {
		std::ifstream file(sourceFile, std::ios::binary);
		if(!file) {
			std::cerr << "Error: Failed to open the file \"" + sourceFile + "\"\n";
			exit(3);
		}

		auto data = std::make_unique<uint16_t[]>(size);

		file.read(reinterpret_cast<char*>(data.get()), size * 2);
		if (!file) {
			std::cerr << "Error: Failed to read the file \"" + sourceFile + "\"\n";
			exit(3);
		}
		file.close();

		for (std::size_t i = 0; i < size; i++)
			data[i] = swapBytes(data[i]);

		{
			std::lock_guard<std::mutex> lock(requestsMutex);
			bufferRequests.push({this, std::move(data)});
		}
	}).detach();
}

void HeightMap::unload() {
	if(state != State::LOADED) return;

	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

	state = State::UNLOADED;
}

void HeightMap::render() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[lod]);
	glDrawElements(GL_TRIANGLES, eboSize[lod], GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	AppData::trianglesDrawn += static_cast<std::size_t>(eboSize[lod] / 3);
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
} 

void HeightMap::DeleteEbos() {
    glDeleteBuffers(9, &ebo[0]);
}



void HeightMap::GenerateRequestedBuffers() {
	while(!bufferRequests.empty()) {
		auto& request = bufferRequests.front();
		auto& vbo = request.target->vbo;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(size * sizeof(uint16_t)), request.data.get());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		request.target->state = State::LOADED;

		bufferRequests.pop();
	}
}
