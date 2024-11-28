#pragma once

#include "../core/scene.hpp"
#include "../graphics/mesh.hpp"

class TestScene : public Scene {
public:
	TestScene();

	void update() override;
	void render() override;

	Mesh testMesh;
};
