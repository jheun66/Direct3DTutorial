#include "Framework.h"

SpriteEffect::SpriteEffect(wstring diffuseFile, Float2 maxFrame)
	:Particle(diffuseFile, 1), time(0), speed(1), curFrameCount(0)
{
	material->SetShader(L"VertexBillboard", L"PixelSprite");
	geometryShader = Shader::AddGS(L"GeometryBillboard");

	this->diffuseFile = diffuseFile;
	spriteBuffer = new SpriteBuffer();
	spriteBuffer->data.maxFrame = maxFrame;
	maxFrameCount = (UINT)(maxFrame.x * maxFrame.y);

	scale = { 0.1f, 0.1f, 0.1f };

	
	Create();



	// 검은색이 나옴
	//blendState[1]->Alpha(true);

}

SpriteEffect::~SpriteEffect()
{
	delete spriteBuffer;
	delete vertices;
}

void SpriteEffect::Create()
{
	vertices = new VertexSize();

	Texture* texture = Texture::Add(diffuseFile);
	float width = texture->GetWidth() / spriteBuffer->data.maxFrame.x * scale.x;
	float height = texture->GetHeight() / spriteBuffer->data.maxFrame.y * scale.y;
	vertices->size = { width, height };

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), 1);

}

void SpriteEffect::Update()
{
	if (!isPlay)
		return;

	time += speed * DELTA;

	if (time > 0.1f)
	{
		curFrameCount++;
		spriteBuffer->data.curFrame.x = curFrameCount % (UINT)spriteBuffer->data.maxFrame.x;
		spriteBuffer->data.curFrame.y = curFrameCount / (UINT)spriteBuffer->data.maxFrame.y;
		time = 0.0f;
	}

	if(curFrameCount > maxFrameCount)
		Stop();

	UpdateWorld();
}

void SpriteEffect::Render()
{
	if (!isPlay)
		return;

	spriteBuffer->SetPSBuffer(10);

	Particle::Render();
}

void SpriteEffect::PostRender()
{

}

void SpriteEffect::Play(Vector3 position)
{
	isPlay = true;

	time = 0.0f;
	curFrameCount = 0;

	vertices->position = position;
	
	vertexBuffer->Update(vertices, 1);
}
