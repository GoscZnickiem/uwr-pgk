#include "sceneManager.hpp"

#include <iostream>

void SceneManager::switchToNextScene() {
	if (m_sceneQueueBuffer.empty()) {
		std::cerr<< "Attempted to switch scene when no scenes in queue are present!\n";
		return;
	}

	if (m_lock) return;

	m_changeScene = true;
	m_bufferScene = m_currentScene;
	m_currentScene = m_sceneQueueBuffer.front();
	m_sceneQueueBuffer.pop();
}

void SceneManager::lock() {
	m_lock = true;
}

void SceneManager::unlock() {
	m_lock = false;
}

bool SceneManager::isLocked() const {
	return m_lock;
}

void SceneManager::atWindowResize(int width, int height) {
	for(auto& cam : m_currentScene->cameras) {
		cam.updateResolution(width, height);
	}
}

void SceneManager::update() {
	m_currentScene->update();
	if (m_changeScene) {
		m_bufferScene.reset();
		m_changeScene = false;
	}
}

void SceneManager::render() {
	m_currentScene->render();
}

