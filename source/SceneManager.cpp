#include "SceneManager.h"

//SceneManager* SceneManager::GetInstance()
//{
//	static SceneManager* instance = nullptr;
//	if (instance == nullptr)
//	{
//		instance = new SceneManager();
//	}
//	return instance;
//}
//
//SceneManager::SceneManager()
//	: m_pScenes{}
//	, m_ActiveSceneIdx{-1}
//{
//}
//SceneManager::~SceneManager()
//{
//	for (SceneGraph* pScene : m_pScenes)
//	{
//		delete pScene;
//		pScene = nullptr;
//	}
//}
//
//void SceneManager::AddScene(SceneGraph* pScene)
//{
//	if (pScene == nullptr)
//	{
//		return;
//	}
//	if (m_ActiveSceneIdx == -1)
//	{
//		m_ActiveSceneIdx = 0;
//	}
//	m_pScenes.push_back(pScene);
//}
//const std::vector<SceneGraph*>& SceneManager::GetScenes()
//{
//	return m_pScenes;
//}
//SceneGraph* SceneManager::GetActiveScene()
//{
//	if (m_ActiveSceneIdx == -1)
//	{
//		return nullptr;
//	}
//
//	return m_pScenes[m_ActiveSceneIdx];
//}
//
//void SceneManager::GotoNextScene()
//{
//	if (m_ActiveSceneIdx == -1)
//	{
//		return;
//	}
//
//	m_ActiveSceneIdx = (m_ActiveSceneIdx++) % m_pScenes.size();
//}
//void SceneManager::GotoPreviousScene()
//{
//	if (m_ActiveSceneIdx == -1)
//	{
//		return;
//	}
//
//	m_ActiveSceneIdx = (m_ActiveSceneIdx--) % m_pScenes.size();
//}
//void SceneManager::GotoScene(unsigned int idx)
//{
//	if (m_ActiveSceneIdx == -1)
//	{
//		return;
//	}
//
//	m_ActiveSceneIdx = idx % m_pScenes.size();
//}
//int SceneManager::GetActiveSceneIndex() const
//{
//	return m_ActiveSceneIdx;
//}

void SceneManager::AddSceneGraph(const SceneGraph& sceneGraph)
{
	m_SceneGraphs.push_back(sceneGraph);
}

SceneGraph& SceneManager::GetActiveScene()
{
	return m_SceneGraphs[m_CurrentlyActiveSceneIndex];
}
SceneGraph& SceneManager::GotoScene(const int index)
{
	m_CurrentlyActiveSceneIndex = index % static_cast<int>(m_SceneGraphs.size());
	return GetActiveScene();
}
SceneGraph& SceneManager::GotoNextScene()
{
	return GotoScene(m_CurrentlyActiveSceneIndex + 1);
}
SceneGraph& SceneManager::GotoPreviousScene()
{
	return GotoScene(m_CurrentlyActiveSceneIndex - 1);
}


