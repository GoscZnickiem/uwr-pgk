#include "aquarium.hpp"
#include "../core/appdata.hpp"

Aquarium::Aquarium(float width, float height, float depth) {
	constexpr float wallWidth = 0.1f;
	constexpr float baseHeight = 0.5f;
	constexpr float baseExtent = 0.3f;

	wall1.mesh = &AppData::Data().models.cube;
	wall1.material = &AppData::Data().materials.glass;
	wall1.transform = &wall1T;
	wall1T.position = {-depth - wallWidth, 0.0f, 0.0f};
	wall1T.scale = {0.1f, height, width};

	wall2.mesh = &AppData::Data().models.cube;
	wall2.material = &AppData::Data().materials.glass;
	wall2.transform = &wall2T;
	wall2T.position = {depth + wallWidth, 0.0f, 0.0f};
	wall2T.scale = {0.1f, height, width};

	wall3.mesh = &AppData::Data().models.cube;
	wall3.material = &AppData::Data().materials.glass;
	wall3.transform = &wall3T;
	wall3T.position = {0.0f, 0.0f, width + wallWidth};
	wall3T.scale = {depth + wallWidth * 2, height, 0.1f};

	wall4.mesh = &AppData::Data().models.cube;
	wall4.material = &AppData::Data().materials.glass;
	wall4.transform = &wall4T;
	wall4T.position = {0.0f, 0.0f, -width - wallWidth};
	wall4T.scale = {depth + wallWidth * 2, height, 0.1f};

	floor.mesh = &AppData::Data().models.plane;
	floor.material = &AppData::Data().materials.test;
	floor.transform = &floorT;
	floorT.position = {0.0f, -height + baseHeight + 0.01f, 0.0f};
	floorT.scale = {depth, 1.0f, width};

	base.mesh = &AppData::Data().models.cube;
	base.material = &AppData::Data().materials.aquariumBase;
	base.transform = &baseT;
	baseT.position = {0.0f, -height + baseHeight/2, 0.0f};
	baseT.scale = {depth + baseExtent, 1.0f, width + baseExtent};

	table.mesh = &AppData::Data().models.plane;
	table.material = &AppData::Data().materials.wood;
	table.transform = &tableT;
	tableT.position = {0.0f, -height - baseHeight, 0.0f};
	tableT.scale = {100.f, 1.0f, 100.f};
}

void Aquarium::render() {
	AppData::Data().renderer.addRender(wall1);
	AppData::Data().renderer.addRender(wall2);
	AppData::Data().renderer.addRender(wall3);
	AppData::Data().renderer.addRender(wall4);
	AppData::Data().renderer.addRender(floor);
	AppData::Data().renderer.addRender(base);
	AppData::Data().renderer.addRender(table);
}
