#include "testScene.hpp"
#include "../core/appdata.hpp"
#include "../core/input.hpp"

TestScene::TestScene()
: a(5.f, 7.f, 10.f) {
	cameras.emplace_back();
	cameras[0].direction = {-1.f, 0.f, 0.f};
	cameras[0].position = {2.f, 0.f, 0.f};
	cameras[0].outsideMode = true;
	Input::setMousePosLock(true);

	test.mesh = &AppData::Data().models.ball;
	test.material = &AppData::Data().materials.test;
	test.transform = &testT;

	test2.mesh = &AppData::Data().models.ball;
	test2.material = &AppData::Data().materials.bubble;
	test2.transform = &testT2;
	testT2.position = {2.0f, 1.0f, 1.0f};
	testT2.scale = {1.0f, 1.0f, 1.0f};

	test3.mesh = &AppData::Data().models.cube;
	test3.material = &AppData::Data().materials.test;
	test3.transform = &testT3;
	testT3.position = {-3.f, 0.0f, 0.3f};
	testT3.rotation = {0.9f, 0.0f, -0.7f};

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
	AppData::Data().renderer.addRender(test3);
	a.render();
	AppData::Data().renderer.render();
}
