#pragma once

class TextureScene : public Scene
{
private:
	Quad* quad;
	Cube* cube;

public:
	TextureScene();
	~TextureScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};