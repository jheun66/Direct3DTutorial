#pragma once

class TessellationTriangleScene : public Scene
{
private:
	Material* material;
	HullShader* hullShader;
	DomainShader* domainShader;

	VertexBuffer* vertexBuffer;

	RasterizerState* rsState[2];

	SettingBuffer* settingBuffer;
public:
	TessellationTriangleScene();
	~TessellationTriangleScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};