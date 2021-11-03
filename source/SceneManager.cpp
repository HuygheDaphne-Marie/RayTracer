#include "SceneManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager* instance = nullptr;
	if (instance == nullptr)
	{
		instance = new SceneManager();
	}
	return instance;
}

SceneManager::SceneManager()
	: m_pScenes{}
	, m_ActiveSceneIdx{-1}
{
}
SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pScenes)
	{
		delete pScene;
		pScene = nullptr;
	}
}

void SceneManager::AddScene(Scene* pScene)
{
	if (pScene == nullptr)
	{
		return;
	}
	if (m_ActiveSceneIdx == -1)
	{
		m_ActiveSceneIdx = 0;
	}
	m_pScenes.push_back(pScene);
}
const std::vector<Scene*>& SceneManager::GetScenes()
{
	return m_pScenes;
}
Scene* SceneManager::GetActiveScene()
{
	if (m_ActiveSceneIdx == -1)
	{
		return nullptr;
	}

	return m_pScenes[m_ActiveSceneIdx];
}

void SceneManager::NextScene()
{
	if (m_ActiveSceneIdx == -1)
	{
		return;
	}

	m_ActiveSceneIdx = (m_ActiveSceneIdx++) % m_pScenes.size();
}
void SceneManager::PreviousScene()
{
	if (m_ActiveSceneIdx == -1)
	{
		return;
	}

	m_ActiveSceneIdx = (m_ActiveSceneIdx--) % m_pScenes.size();
}
void SceneManager::SetActiveSceneIndex(unsigned int idx)
{
	if (m_ActiveSceneIdx == -1)
	{
		return;
	}

	m_ActiveSceneIdx = idx % m_pScenes.size();
}
int SceneManager::GetActiveSceneIndex() const
{
	return m_ActiveSceneIdx;
}
