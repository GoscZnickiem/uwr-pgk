#include "aquarium.hpp"
#include "../core/appdata.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>

Aquarium::Aquarium(float width, float height, float depth) {
	constexpr float wallWidth = 0.1f;
	constexpr float baseHeight = 3.0f;
	constexpr float baseExtent = 0.3f;
	constexpr float waterGap = 0.5f;

	wall1.mesh = &AppData::Data().models.cube;
	wall1.material = &AppData::Data().materials.glass;
	wall1.transform = &wall1T;
	wall1T.position = {-depth - wallWidth, 0.0f, 0.0f};
	wall1T.scale = {wallWidth, height, width};

	wall2.mesh = &AppData::Data().models.cube;
	wall2.material = &AppData::Data().materials.glass;
	wall2.transform = &wall2T;
	wall2T.position = {depth + wallWidth, 0.0f, 0.0f};
	wall2T.scale = {wallWidth, height, width};

	wall3.mesh = &AppData::Data().models.cube;
	wall3.material = &AppData::Data().materials.glass;
	wall3.transform = &wall3T;
	wall3T.position = {0.0f, 0.0f, width + wallWidth};
	wall3T.scale = {depth + wallWidth * 2, height, wallWidth};

	wall4.mesh = &AppData::Data().models.cube;
	wall4.material = &AppData::Data().materials.glass;
	wall4.transform = &wall4T;
	wall4T.position = {0.0f, 0.0f, -width - wallWidth};
	wall4T.scale = {depth + wallWidth * 2, height, wallWidth};

	water1.mesh = &AppData::Data().models.plane;
	water1.material = &AppData::Data().materials.water;
	water1.transform = &water1T;
	water1T.position = {-depth, -waterGap, 0.0f};
	water1T.rotation = {glm::half_pi<float>(), glm::half_pi<float>(), 0.0f};
	water1T.scale = {width, 1.0f, height - waterGap};

	water2.mesh = &AppData::Data().models.plane;
	water2.material = &AppData::Data().materials.water;
	water2.transform = &water2T;
	water2T.position = {depth, -waterGap, 0.0f};
	water2T.rotation = {-glm::half_pi<float>(), glm::half_pi<float>(), 0.0f};
	water2T.scale = {width, 1.0f, height - waterGap};

	water3.mesh = &AppData::Data().models.plane;
	water3.material = &AppData::Data().materials.water;
	water3.transform = &water3T;
	water3T.position = {0.0f, -waterGap, -width};
	water3T.rotation = {0.0f, glm::half_pi<float>(), 0.0f};
	water3T.scale = {depth, 1.0f, height - waterGap};

	water4.mesh = &AppData::Data().models.plane;
	water4.material = &AppData::Data().materials.water;
	water4.transform = &water4T;
	water4T.position = {0.0f, -waterGap, width};
	water4T.rotation = {glm::pi<float>(), glm::half_pi<float>(), 0.0f};
	water4T.scale = {depth, 1.0f, height - waterGap};

	waterA.mesh = &AppData::Data().models.plane;
	waterA.material = &AppData::Data().materials.water;
	waterA.transform = &waterAT;
	waterAT.position = {0.0f, height - waterGap * 2, 0.0f};
	waterAT.scale = {depth, 1.0f, width};

	waterB.mesh = &AppData::Data().models.plane;
	waterB.material = &AppData::Data().materials.water;
	waterB.transform = &waterBT;
	waterBT.position = {0.0f, height - waterGap * 2, 0.0f};
	waterBT.rotation = {0.0f, glm::pi<float>(), 0.0f};
	waterBT.scale = {depth, 1.0f, width};

	floor.mesh = &AppData::Data().models.plane;
	floor.material = &AppData::Data().materials.gravel;
	floor.transform = &floorT;
	floorT.position = {0.0f, -height + 0.01f, 0.0f};
	floorT.scale = {depth, 1.0f, width};

	base.mesh = &AppData::Data().models.cube;
	base.material = &AppData::Data().materials.aquariumBase;
	base.transform = &baseT;
	baseT.position = {0.0f, -height - baseHeight - 0.7f, 0.0f};
	baseT.scale = {depth + baseExtent * 2, baseHeight, width + baseExtent};

	table.mesh = &AppData::Data().models.plane;
	table.material = &AppData::Data().materials.wood;
	table.transform = &tableT;
	tableT.position = {0.0f, -height - baseHeight * 3, 0.0f};
	tableT.scale = {100.f, 1.0f, 100.f};
}

void Aquarium::render() {
	AppData::Data().renderer.addRender(wall1);
	AppData::Data().renderer.addRender(wall2);
	AppData::Data().renderer.addRender(wall3);
	AppData::Data().renderer.addRender(wall4);

	AppData::Data().renderer.addRender(water1);
	AppData::Data().renderer.addRender(water2);
	AppData::Data().renderer.addRender(water3);
	AppData::Data().renderer.addRender(water4);
	AppData::Data().renderer.addRender(waterA);
	AppData::Data().renderer.addRender(waterB);

	AppData::Data().renderer.addRender(floor);
	AppData::Data().renderer.addRender(base);
	AppData::Data().renderer.addRender(table);
}

void Aquarium::fogRender() {
	AppData::Data().renderer.addRender(water1);
	AppData::Data().renderer.addRender(water2);
	AppData::Data().renderer.addRender(water3);
	AppData::Data().renderer.addRender(water4);
	AppData::Data().renderer.addRender(waterB);
	AppData::Data().renderer.addRender(floor);
}
