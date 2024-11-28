#ifndef _GZN_PGK_CORE_SCENE_
#define _GZN_PGK_CORE_SCENE_

#include "../gameobjects/camera.hpp"
#include <memory>
#include <vector>

class Scene : public std::enable_shared_from_this<Scene>  {
public:
	Scene() = default;
	virtual ~Scene() = default;
	Scene(const Scene &) = delete;
	Scene(Scene &&) = delete;
	Scene &operator=(const Scene &) = delete;
	Scene &operator=(Scene &&) = delete;

	virtual void update() = 0;
	virtual void render() = 0;

	std::vector<Camera> cameras;
};

#endif
