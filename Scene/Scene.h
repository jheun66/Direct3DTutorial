#pragma once

class Scene
{
public:
	// �Ҹ��ڸ� �����Լ��� ����� ����(��ĳ��Ʈ�� �Ҹ��� ȣ���Ҷ� .. )
	virtual ~Scene() = default;

	virtual void Update() = 0;

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

};