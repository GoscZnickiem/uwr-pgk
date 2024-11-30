#include "testScene.hpp"
#include "../core/appdata.hpp"
#include "../core/input.hpp"

TestScene::TestScene() {
	cameras.emplace_back();
	cameras[0].direction = {-1.f, 0.f, 0.f};
	cameras[0].position = {2.f, 0.f, 0.f};
	cameras[0].outsideMode = true;
	Input::setMousePosLock(true);

	test.mesh = &AppData::Data().ball;
	test.material = &AppData::Data().ballMat2;
	test.transform = &testT;

	test2.mesh = &AppData::Data().ball;
	test2.material = &AppData::Data().ballMat2;
	test2.transform = &testT2;
	testT2.position = {1.f, 1.f, 1.0f};

	globalLight.direction = glm::normalize(glm::vec3{0.2f, -1.f, 0.1f});
	globalLight.color = {1.f, 1.f, 1.f};
	globalLight.intensity = 1.0f;
}

void TestScene::update() {
	cameras[0].update({0,0,0});
}

void TestScene::render() {
	cameras[0].setup();
	Shader::SetLightDirectional(globalLight);
	Shader::SetLightsUniform();
	AppData::Data().renderer.addRender(test);
	AppData::Data().renderer.addRender(test2);
	AppData::Data().renderer.render();
}
