#include "renderer.hpp"
#include <algorithm>
#include <cstdint>
#include <tuple>

void Renderer::render() {
	const Shader* currentShader = nullptr;
	Mesh* currentMesh = nullptr;
	Material* currentMaterial = nullptr;
	for(auto& [data, arr] : batches) {
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
			// data.material.setup();
		}
		if(currentMaterial->opacity != 1.f) {
			std::sort(arr.begin(), arr.end(), [&](const glm::mat4& a, const glm::mat4& b){
				auto distax = a[3].x - cameraPosition.x;
				auto distay = a[3].y - cameraPosition.y;
				auto distaz = a[3].z - cameraPosition.z;
				auto distbx = b[3].x - cameraPosition.x;
				auto distby = b[3].y - cameraPosition.y;
				auto distbz = b[3].z - cameraPosition.z;
				return distax * distax + distay * distay + distaz * distaz < distbx * distbx + distby * distby + distbz * distbz;
				// na moim komputerze glm/gtx/norm.hpp nie działa więc nie mam dostępu do glm::lenght2
			});
		}
		data.mesh->setTransforms(arr.data(), arr.size());
		data.mesh->render();
		arr.clear();
	}
}

bool Renderer::RenderDataIndex::operator<(const RenderDataIndex& other) const {
	auto shaderID = reinterpret_cast<uintptr_t>(material->shader);
	auto meshID = reinterpret_cast<uintptr_t>(mesh);
	auto materialID = reinterpret_cast<uintptr_t>(material);
	auto oshaderID = reinterpret_cast<uintptr_t>(other.material->shader);
	auto omeshID = reinterpret_cast<uintptr_t>(other.mesh);
	auto omaterialID = reinterpret_cast<uintptr_t>(other.material);
	return std::tie(shaderID, meshID, materialID) < std::tie(oshaderID, omeshID, omaterialID);
}

Renderer::RenderDataIndex::RenderDataIndex(const Renderable& renderData)
: mesh(renderData.mesh), material(renderData.material) { }

void Renderer::addRender(Renderable renderData) {
	batches[{renderData}].push_back(renderData.transform->getMatrix());
}

void Renderer::setCameraPos(const glm::vec3& pos) {
	cameraPosition = pos;
}
