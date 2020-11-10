#pragma once

class Program
{
private:
	//Scene* scene;

public:
	Program();
	~Program();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};