#include "appdata.hpp"

#include <iostream>
#include <memory>

static std::unique_ptr<AppData> intance;

AppData& AppData::Data() {
	return *intance;
}

void AppData::Init() {
	if (glfwInit() == 0) {
		std::cerr << "GLFW initialization failed\n";
		exit(1);
	}
	intance = std::make_unique<AppData>(Token{});
}

void AppData::Terminate() {
	intance.reset();
	glfwTerminate();
}

void AppData::atResize(int width, int height) {
	glViewport(0, 0, width, height);
	sceneManager.atWindowResize(width, height);
	renderer.updateFrameBuffers(width, height);
}

AppData::AppData([[maybe_unused]] Token t) :
	mainLoop{}, window{}, sceneManager{}, renderer{},
	shaders {
		.standard {"assets/shaders/shader.glsl"},
		.waterDepth {"assets/shaders/waterDepth.glsl"},
		.quad {"assets/shaders/quad.glsl"}
	}, models {
		.ball {Mesh::CreateSphereMesh(4)},
		.cube {
			{
				-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,
				 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,
				 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,
				-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,

				-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,
				 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,
				 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,
				-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,

				-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,
				-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,
				-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,
				-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,

				 1.0f, -1.0f, -1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,
				 1.0f,  1.0f, -1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,
				 1.0f,  1.0f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,
				 1.0f, -1.0f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,

				-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,
				 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,
				 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,
				-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,

				-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
				 1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
				 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
				-1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
			}, 
			{
				0, 2, 1,  0, 3, 2,
				4, 5, 6,  4, 6, 7,
				8, 10, 9,  8, 11, 10,
				12, 13, 14, 12, 14, 15,
				16, 17, 18, 16, 18, 19,
				20, 22, 21, 20, 23, 22
			}
		},
		.cubeInv {
			{
				-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,
				 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,
				 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,
				-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.f, 0.f,

				-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,
				 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,
				 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,
				-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.f, 0.f,

				-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,
				-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,
				-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,
				-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.f, 0.f,

				 1.0f, -1.0f, -1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,
				 1.0f,  1.0f, -1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,
				 1.0f,  1.0f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,
				 1.0f, -1.0f,  1.0f, 	 1.0f,  0.0f,  0.0f, 	0.f, 0.f,

				-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,
				 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,
				 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,
				-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.f, 0.f,

				-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
				 1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
				 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
				-1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.f, 0.f,
			}, 
			{
				0, 1, 2,  0, 2, 3,
				4, 6, 5,  4, 7, 6,
				8, 9, 10,  8, 10, 11,
				12, 14, 13, 12, 15, 14,
				16, 18, 17, 16, 19, 18,
				20, 21, 22, 20, 22, 23
			}
		},
		.plane {
			{
				-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
				-1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
				 1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
				 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
			},
			{
				0, 1, 2, 0, 2, 3
			}
		}
	}, materials {
		.bubble {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{1.0f, 1.0f, 1.3f}, 
			.diffuse  = glm::vec3{0.7f, 0.8f, 1.4f}, 
			.specular = glm::vec3{1.0f, 1.0f, 1.2f}, 
			.shininess = 32.f, .opacity = 0.5f, .alpha = 1.0f
		},
		.light {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{50.0f, 50.0f, 50.0f}, 
			.diffuse  = glm::vec3{2.0f, 2.0f, 2.0f}, 
			.specular = glm::vec3{2.0f, 2.0f, 2.0f}, 
			.shininess = 32.f, .opacity = 1.0f, .alpha = 1.0f
		},
		.glass {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{1.0f, 1.3f, 1.1f}, 
			.diffuse  = glm::vec3{1.0f, 1.3f, 1.1f}, 
			.specular = glm::vec3{2.0f, 2.0f, 2.0f}, 
			.shininess = 128.f, .opacity = 0.3f, .alpha = 0.2f
		},
		.wood {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{0.7f, 0.6f, 0.2f}, 
			.diffuse  = glm::vec3{0.7f, 0.6f, 0.2f}, 
			.specular = glm::vec3{0.9f, 0.9f, 0.8f}, 
			.shininess = 64.f, .opacity = 1.0f, .alpha = 1.0f
		},
		.aquariumBase {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{0.1f, 0.1f, 0.1f}, 
			.diffuse  = glm::vec3{0.1f, 0.1f, 0.1f}, 
			.specular = glm::vec3{0.8f, 0.8f, 0.8f}, 
			.shininess = 64.f, .opacity = 1.0f, .alpha = 1.0f
		},
		.gravel {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{0.3f, 0.3f, 0.4f}, 
			.diffuse  = glm::vec3{0.5f, 0.5f, 0.5f}, 
			.specular = glm::vec3{0.1f, 0.1f, 0.1f}, 
			.shininess = 4.f, .opacity = 1.0f, .alpha = 1.0f
		},
		.water {
			.shader = &shaders.standard, 
			.ambient  = glm::vec3{0.8f, 0.7f, 1.0f}, 
			.diffuse  = glm::vec3{0.5f, 0.7f, 0.9f}, 
			.specular = glm::vec3{0.8f, 1.0f, 1.2f}, 
			.shininess = 64.f, .opacity = 0.3f, .alpha = 0.9f
		}
	} {}
