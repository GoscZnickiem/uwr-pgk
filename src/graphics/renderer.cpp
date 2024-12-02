#include "renderer.hpp"
#include "../core/appdata.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <tuple>

Renderer::Renderer() {
	glGenTextures(1, &waterDepthTexture);
	glBindTexture(GL_TEXTURE_2D, waterDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 800, 600, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, waterDepthTexture, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "frame buffer error\n";
		exit(3);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::render() {
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterDepthTexture);

	const Shader* currentShader = nullptr;
	Mesh* currentMesh = nullptr;
	Material* currentMaterial = nullptr;
	for(auto& [data, arr] : opaqueBatches) {
		if(currentShader != data.material->shader) {
			currentShader = data.material->shader;
			currentShader->bind();
			currentShader->setUniform("depthTexture", 0);
		}
		if(currentMesh != data.mesh) {
			currentMesh = data.mesh;
			currentMesh->bind();
		}
		if(currentMaterial != data.material) {
			currentMaterial = data.material;
			currentMaterial->setup();
		}
		currentMesh->setTransforms(arr.data(), arr.size());
		currentMesh->render();
		arr.clear();
	}

	std::sort(translucentQueue.begin(), translucentQueue.end(), [&](const Renderable& a, const Renderable& b){
		auto da = a.transform->position - cameraPosition;
		auto db = b.transform->position - cameraPosition;
		return glm::length(da) > glm::length(db);
	});

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	auto it = translucentQueue.begin();
	if(it == translucentQueue.end()) return;
	for(;it != translucentQueue.end(); it++) {
		bool shaderChange = currentShader != it->material->shader;
		bool meshChange = currentMesh != it->mesh;
		bool materialChange = currentMaterial != it->material;
		if(shaderChange || meshChange || materialChange) {
			if(translucentArray.size() != 0) {
				currentMesh->setTransforms(translucentArray.data(), translucentArray.size());
				currentMesh->render();
				translucentArray.clear();
			}
			if(shaderChange) {
				currentShader = it->material->shader;
				currentShader->bind();
				currentShader->setUniform("depthTexture", 0);
			}
			if(meshChange) {
				currentMesh = it->mesh;
				currentMesh->bind();
			}
			if(materialChange) {
				currentMaterial = it->material;
				currentMaterial->setup();
			}
		}
		translucentArray.push_back(it->transform->getMatrix());
	}
	glDepthMask(GL_TRUE);
	currentMesh->setTransforms(translucentArray.data(), translucentArray.size());
	currentMesh->render();
	translucentArray.clear();
	translucentQueue.clear();

	float quadVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f
	};

	static GLuint quadVAO, quadVBO = 0;
	if(quadVAO == 0) {
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	}

	AppData::Data().shaders.quad.bind();
	AppData::Data().shaders.quad.setUniform("depthTexture", 0);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::writeDepth() {
	glDisable(GL_CULL_FACE); 
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	// glBlendFunc(GL_MIN, GL_MAX);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	AppData::Data().shaders.waterDepth.bind();

	// Mesh* currentMesh = nullptr;
	// for(auto& [data, arr] : opaqueBatches) {
	// 	if(currentMesh != data.mesh) {
	// 		currentMesh = data.mesh;
	// 		currentMesh->bind();
	// 	}
	// 	currentMesh->setTransforms(arr.data(), arr.size());
	// 	currentMesh->render();
	// 	arr.clear();
	// }
	//
	// auto it = translucentQueue.begin();
	// if(it == translucentQueue.end()) return;
	// for(;it != translucentQueue.end(); it++) {
	// 	bool meshChange = currentMesh != it->mesh;
	// 	if(meshChange) {
	// 		if(translucentArray.size() != 0) {
	// 			currentMesh->setTransforms(translucentArray.data(), translucentArray.size());
	// 			currentMesh->render();
	// 			translucentArray.clear();
	// 		}
	// 		currentMesh = it->mesh;
	// 		currentMesh->bind();
	// 	}
	// 	translucentArray.push_back(it->transform->getMatrix());
	// }
	// currentMesh->setTransforms(translucentArray.data(), translucentArray.size());
	// currentMesh->render();
	// translucentArray.clear();
	// translucentQueue.clear();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE); 
}

void Renderer::addRender(Renderable renderData) {
	if(renderData.material->opacity == 1.f && renderData.material->alpha == 1.f)
		opaqueBatches[{renderData}].push_back(renderData.transform->getMatrix());
	else
		translucentQueue.push_back(renderData);
}

void Renderer::setCameraPos(const glm::vec3& pos) {
	cameraPosition = pos;
}

void Renderer::updateFrameBuffers(int width, int height) {
	glBindTexture(GL_TEXTURE_2D, waterDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, waterDepthTexture, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "frame buffer error\n";
		exit(3);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Renderer::OpaqueRenderIndex::operator<(const OpaqueRenderIndex& other) const {
	auto shaderID = reinterpret_cast<uintptr_t>(material->shader);
	auto meshID = reinterpret_cast<uintptr_t>(mesh);
	auto materialID = reinterpret_cast<uintptr_t>(material);
	auto oshaderID = reinterpret_cast<uintptr_t>(other.material->shader);
	auto omeshID = reinterpret_cast<uintptr_t>(other.mesh);
	auto omaterialID = reinterpret_cast<uintptr_t>(other.material);
	return std::tie(shaderID, meshID, materialID) < std::tie(oshaderID, omeshID, omaterialID);
}
Renderer::OpaqueRenderIndex::OpaqueRenderIndex(const Renderable& renderData)
: mesh(renderData.mesh), material(renderData.material) { }
