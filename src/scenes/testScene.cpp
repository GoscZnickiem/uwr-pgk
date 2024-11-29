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
	test.material = &AppData::Data().ballMat;
	test.transform = &testT;

	test2.mesh = &AppData::Data().ball;
	test2.material = &AppData::Data().ballMat;
	test2.transform = &testT2;
	testT2.position = {1.f, 0.f, 0.3f};
}

void TestScene::update() {
	cameras[0].update({0,0,0});
}

void TestScene::render() {
	cameras[0].setup();
	AppData::Data().renderer.addRender(test);
	AppData::Data().renderer.addRender(test2);
	AppData::Data().renderer.render();
}
