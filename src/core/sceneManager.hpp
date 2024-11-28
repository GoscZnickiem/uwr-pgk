#ifndef _GZN_PGK_CORE_SCENEMANAGER_
#define _GZN_PGK_CORE_SCENEMANAGER_

#include "scene.hpp"
#include <memory>
#include <queue>
#include <utility>

class SceneManager {	
public:
	void update();
	void render();

	template<typename SceneType, typename...Args>
	void pushScene(Args&&... args) {
		if(m_lock) return;
		static_assert(std::is_base_of_v<Scene, SceneType>, "SceneType must derive from Scene");
		m_sceneQueueBuffer.push(std::static_pointer_cast<Scene>(std::make_shared<SceneType>(std::forward<Args>(args)...)));
	}
	void switchToNextScene();
	std::shared_ptr<Scene>& getCurrentScene();
	std::queue<std::shared_ptr<Scene>>& getSceneQueue();

	void lock();
	void unlock();
	[[nodiscard]] bool isLocked() const;

	void atWindowResize(int width, int height);

private:
	bool m_changeScene = false;
	bool m_lock = false;

	std::shared_ptr<Scene> m_bufferScene;
	std::shared_ptr<Scene> m_currentScene;
	std::queue<std::shared_ptr<Scene>> m_sceneQueueBuffer;	
};

#endif
