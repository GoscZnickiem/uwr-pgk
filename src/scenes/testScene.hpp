#pragma once

#include "../core/scene.hpp"
#include "../graphics/renderable.hpp"

class TestScene : public Scene {
public:
	TestScene();

	void update() override;
	void render() override;

	Transform testT;
	Renderable test;
	Transform testT2;
	Renderable test2;

	Light globalLight;
};
