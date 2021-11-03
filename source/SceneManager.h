#pragma once
#include <vector>
#include "Scene.h"

#define SCENEMANAGER SceneManager::GetInstance()

class SceneManager final
{
public:
	static SceneManager* GetInstance();
	~SceneManager();

	void AddScene(Scene* pScene);
	const std::vector<Scene*>& GetScenes();
	Scene* GetActiveScene();

	void NextScene();
	void PreviousScene();
	void SetActiveSceneIndex(unsigned int idx);
	int GetActiveSceneIndex() const;

private:
	SceneManager();

	std::vector<Scene*> m_pScenes;
	int m_ActiveSceneIdx;
};

