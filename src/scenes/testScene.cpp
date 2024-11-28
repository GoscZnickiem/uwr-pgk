#include "testScene.hpp"
#include "../core/appdata.hpp"
#include "../core/input.hpp"
#include "../transform.hpp"

TestScene::TestScene()
: testMesh(Mesh::CreateSphereMesh(3, {1.f, 0.f, 0.f, 0.7f}, {1.f, 1.f, 0.f, 0.7f})) {
	cameras.emplace_back();
	cameras[0].direction = {-1.f, 0.f, 0.f};
	cameras[0].position = {2.f, 0.f, 0.f};
	cameras[0].outsideMode = true;
	// Input::setMousePosLock(true);
	std::vector<glm::mat4> t = { Transform().getMatrix() };
	testMesh.setTransforms(t);
}

void TestScene::update() {
	cameras[0].update({0,0,0});
}

void TestScene::render() {
	cameras[0].setup();
	AppData::Data().shader.bind();
	testMesh.render();
}
