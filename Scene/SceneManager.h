#pragma once

class SceneManager
{
private:
	map<string, Scene*> scenes;

	Scene* curScene;

	static SceneManager* instance;

	SceneManager();
	~SceneManager();
public:
	static SceneManager* Get() { return instance; }
	static void Create() { instance = new SceneManager(); }
	static void Delete() { delete instance; }

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	Scene* AddScene(string key, Scene* scene);
	Scene* ChangeScene(string key);
};