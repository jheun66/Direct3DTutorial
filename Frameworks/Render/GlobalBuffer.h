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


#define MAX_LIGHT 10
struct LightInfo
{
	enum Type
	{
		DIRECTION,
		POINT,
		SPOT,
		CAPSULE
	};

	Float4 color;
	
	Float3 position;
	float range;

	Float3 direction;
	float outer;

	float inner;
	float length;
	Type type;
	int active;

	LightInfo()
	{
		color = Float4(1, 1, 1, 1);
		position = Float3(0, 0, 0);
		range = 80.0f;
		direction = Float3(0, -1, 0);
		outer = 65.0f;
		inner = 55.0f;
		length = 50;
		type = POINT;
		active = 1;
	}

};

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		LightInfo lights[MAX_LIGHT];

		UINT lightCount;
		float padding[3];

		Float4 ambient;
		Float4 ambientCeil;
	}data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.lightCount = 0;

		data.ambient = { 0.1f,0.1f,0.1f,1.0f };
		data.ambientCeil = { 0.1f,0.1f,0.1f,1.0f };
	}

	void Add(LightInfo light)
	{
		data.lights[data.lightCount++] = light;
	}
	void Add()
	{
		data.lightCount++;
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
		int option[4];
		
		Data() :option{}
		{}
	}data;

	SettingBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

};

class TimeBuffer : public ConstBuffer
{
public:
	struct Data
	{
		float time;

		Float3 padding;

		Data() : time(0)
		{}
	}data;

	TimeBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

};

class SizeBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float2 size;
		Float2 padding;

	}data;

	SizeBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.size = { 0, 0 };
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

class BoneBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Matrix bones[MAX_MODEL_BONE];
	}data;

	BoneBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (UINT i = 0; i < MAX_MODEL_BONE; i++)
			data.bones[i] = XMMatrixIdentity();
	}

	void Add(Matrix matrix, UINT index)
	{
		//data.bones[index] = matrix;
		data.bones[index] = XMMatrixTranspose(matrix);
	}
};