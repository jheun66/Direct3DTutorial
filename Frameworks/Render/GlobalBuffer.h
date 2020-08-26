#pragma once
// GlobalBuffer

class MatrixBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix matrix;
	}data;

public:
	MatrixBuffer() : ConstBuffer(&data, sizeof(Data)) 
	{
		data.matrix = XMMatrixIdentity();
	}

	void Set(Matrix value)
	{
		data.matrix = XMMatrixTranspose(value);
	}

};

class ColorBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 color;
	}data;

	ColorBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.color = { 1,1,1,1 };
	}

};

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 direction;

		// Exp 지수
		float specularExp;
		// 주변광 비율
		Float4 ambient;
	}data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.direction = { 0, -1, 0 };
		data.specularExp = 8;
		data.ambient = { 0.1f, 0.1f, 0.1f, 0.0f };
	}

};


class ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix invView;
	}data;

public:
	ViewBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.view = XMMatrixIdentity();
		data.invView = XMMatrixIdentity();
	}

	void Set(Matrix value)
	{
		data.view = XMMatrixTranspose(value);
		Matrix tmp = XMMatrixInverse(nullptr, value);
		data.invView = XMMatrixTranspose(tmp);
	}

};

class SettingBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int option[16];
		
		Data() :option{}
		{}
	}data;

	SettingBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

};

class RayBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 position;
		float size;
		Float3 direction;
		float padding;
	}data;

	RayBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.position = Float3(0, 0, 0);
		data.size = 0;
		data.direction = Float3(0, 0, 0);
	}

};