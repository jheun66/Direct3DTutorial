#pragma once

class Scene
{
public:
	// 소멸자를 가상함수로 만드는 이유(업캐스트시 소멸자 호출할때 .. )
	virtual ~Scene() = default;

	virtual void Update() = 0;

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

};