#include "mainScene.hpp"
#include "core/appdata.hpp"

void MainScene::update() {

}

void MainScene::render() {
	AppData::Data().shaders.map2D.bind();
	AppData::Data().shaders.map2D.setUniform("cameraPos", 0.5f, 0.5f);
	AppData::Data().shaders.map2D.setUniform("scale", 1.f, 1.f);
	AppData::Data().shaders.map2D.setUniform("position", 0, 0);
	AppData::Data().shaders.map2D.setUniform("side", 1201u);
	AppData::Data().shaders.map2D.setUniform("lod", 1u);
	chunks[0]->render(0);
	AppData::Data().shaders.map2D.setUniform("position", 1, 0);
	chunks[1]->render(0);
}
