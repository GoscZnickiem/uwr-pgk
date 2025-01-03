#include "mainScene.hpp"
#include "core/appdata.hpp"

void MainScene::update() {

}

void MainScene::render() {
	AppData::Data().shaders.map2D.bind();
	AppData::Data().shaders.map2D.setUniform("cameraPos", 0.f, 0.f);
	AppData::Data().shaders.map2D.setUniform("scale", 0.001f, 0.0001f);
	AppData::Data().shaders.map2D.setUniform("position", 0, 1);
	AppData::Data().shaders.map2D.setUniform("side", 1201u);
	AppData::Data().shaders.map2D.setUniform("lod", 1u);
	chunks[0]->render(0);
}
