#ifndef _GZN_PGK_GAMEOBJECTS_AQUARIUM_
#define _GZN_PGK_GAMEOBJECTS_AQUARIUM_

#include "../graphics/renderable.hpp"

class Aquarium {
public:
	Aquarium(float width, float height, float depth);

	void render();

private:
	Renderable wall1;
	Transform wall1T;
	Renderable wall2;
	Transform wall2T;
	Renderable wall3;
	Transform wall3T;
	Renderable wall4;
	Transform wall4T;

	Renderable water1;
	Transform water1T;
	Renderable water2;
	Transform water2T;
	Renderable water3;
	Transform water3T;
	Renderable water4;
	Transform water4T;
	Renderable waterA;
	Transform waterAT;
	Renderable waterB;
	Transform waterBT;

	Renderable floor;
	Transform floorT;
	Renderable base;
	Transform baseT;
	Renderable table;
	Transform tableT;
};

#endif