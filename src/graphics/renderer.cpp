#include "renderer.hpp"
#include <algorithm>
#include <cstdint>
#include <tuple>

void Renderer::render() {
	const Shader* currentShader = nullptr;
	Mesh* currentMesh = nullptr;
	Material* currentMaterial = nullptr;
	for(auto& [data, arr] : opaqueBatches) {
		if(currentShader != data.material->shader) {
			currentShader = data.material->shader;
			currentShader->bind();
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
		return da.x * da.x + da.y * da.y + da.z * da.z > db.x * db.x + db.y * db.y + db.z * db.z;
		// na moim komputerze glm/gtx/norm.hpp nie działa więc nie mam dostępu do glm::lenght2
	});

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
	currentMesh->setTransforms(translucentArray.data(), translucentArray.size());
	currentMesh->render();
	translucentArray.clear();
	translucentQueue.clear();
}

void Renderer::addRender(Renderable renderData) {
	if(renderData.material->opacity == 1.f)
		opaqueBatches[{renderData}].push_back(renderData.transform->getMatrix());
	else
		translucentQueue.push_back(renderData);
}

void Renderer::setCameraPos(const glm::vec3& pos) {
	cameraPosition = pos;
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
