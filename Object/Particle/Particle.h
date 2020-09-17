#pragma once

class Particle :public Transform
{
protected:
	Material* material;
	GeometryShader* geometryShader;

	VertexBuffer* vertexBuffer;
	UINT particleCount;
	bool isPlay;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];
public:
	Particle(wstring diffuseFile, UINT particleCount);
	virtual ~Particle();

	virtual void Create() = 0;
	
	virtual void Update() = 0;

	virtual void Render();

	virtual void Play(Vector3 position);
	virtual void Stop();

	bool IsPlay() { return isPlay; }

};