#pragma once

class Outline : public Render2D
{
private:
	SettingBuffer* settingBuffer;
	SizeBuffer* sizeBuffer;

public:
	Outline();
	~Outline();

	virtual void Render() override;
	void PostRender();
};