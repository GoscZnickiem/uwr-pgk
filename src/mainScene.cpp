#include "mainScene.hpp"
#include "core/appdata.hpp"

void MainScene::update() {

}

void MainScene::render() {
	const std::size_t lod = 0;
	AppData::Data().shaders.map2D.bind();
	AppData::Data().shaders.map2D.setUniform("cameraPos", 1.0f, 0.5f);
	AppData::Data().shaders.map2D.setUniform("scale", 1.f, 1.f);
	AppData::Data().shaders.map2D.setUniform("position", 0, 0);
	AppData::Data().shaders.map2D.setUniform("side", 1201u);
	chunks[0]->render(lod);
	AppData::Data().shaders.map2D.setUniform("position", 1, 0);
	chunks[1]->render(lod);
}
