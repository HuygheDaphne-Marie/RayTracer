#pragma once
//#include <vector>
//#include "SceneGraph.h"
//
//#define SCENEMANAGER SceneManager::GetInstance()
//
//class SceneManager final
//{
//public:
//	static SceneManager* GetInstance();
//	~SceneManager();
//
//	void AddScene(SceneGraph* pScene);
//	const std::vector<SceneGraph*>& GetScenes();
//	SceneGraph* GetActiveScene();
//
//	void GotoNextScene();
//	void GotoPreviousScene();
//	void GotoScene(unsigned int idx);
//	int GetActiveSceneIndex() const;
//
//private:
//	SceneManager();
//
//	std::vector<SceneGraph*> m_pScenes;
//	int m_ActiveSceneIdx;
//};

#include <vector>

#include "Singleton.h"
#include "SceneGraph.h"

class SceneManager final : public Singleton<SceneManager>
{
public:
	SceneManager(const SceneManager& other) = delete;
	SceneManager(SceneManager&& other) = delete;
	SceneManager& operator=(const SceneManager& other) = delete;
	SceneManager& operator=(SceneManager&& other) = delete;
	~SceneManager() override = default;

	void AddSceneGraph(const SceneGraph& sceneGraph);

	SceneGraph& GetActiveScene();
	SceneGraph& GotoScene(const int index);
	SceneGraph& GotoNextScene();
	SceneGraph& GotoPreviousScene();

private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;

	int m_CurrentlyActiveSceneIndex = 0;
	std::vector<SceneGraph> m_SceneGraphs{ SceneGraph{} };
};

