#include "Framework.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager() :curScene(nullptr)
{
}

SceneManager::~SceneManager()
{
    for (auto scene : scenes)
        delete scene.second;
}

void SceneManager::Update()
{
    if (curScene != nullptr)
        curScene->Update();
}

void SceneManager::PreRender()
{
    if (curScene != nullptr)
        curScene->PreRender();
}

void SceneManager::Render()
{
    if (curScene != nullptr)
        curScene->Render();
}

void SceneManager::PostRender()
{
    if (curScene != nullptr)
        curScene->PostRender();
}

Scene* SceneManager::AddScene(string key, Scene* scene)
{
    if (scenes.count(key) > 0)
        return scenes[key];

    scenes[key] = scene;

    return scene;
}

Scene* SceneManager::ChangeScene(string key)
{
    if (scenes.count(key) == 0)
        return curScene;

    if (curScene == scenes[key])
        return curScene;

    if(curScene != nullptr)
        curScene->End();

    curScene = scenes[key];

    curScene->Start();

    return nullptr;
}
