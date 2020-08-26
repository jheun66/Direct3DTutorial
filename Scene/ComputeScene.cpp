#include "Framework.h"
#include "ComputeScene.h"

ComputeScene::ComputeScene()
{
	Raw();

}

ComputeScene::~ComputeScene()
{
	
}

void ComputeScene::Update()
{
}

void ComputeScene::PreRender()
{
}

void ComputeScene::Render()
{
}

void ComputeScene::PostRender()
{
}

void ComputeScene::Raw()
{
	ComputeShader* shader = Shader::AddCS(L"ByteAddress");

	struct Output
	{
		UINT groupID[3];
		UINT groupThreadID[3];		// �׷� ���� ID
		UINT dispatchThreadID[3];	// ����ġ ���� ID
		UINT groupIndex;
	};
			//10 * 8 * 3 �� �׷�� ������ ����	������ 2�� Dispatch ��(�׷찳��)
	UINT size = 10 * 8 * 3 * 2;
	Output* output = new Output[size];
	RawBuffer* buffer = new RawBuffer(nullptr, sizeof(Output) * size);

	ID3D11UnorderedAccessView* uav = buffer->GetUAV();

	shader->Set();

	DC->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	// �ΰ��� �׷�
	DC->Dispatch(2, 1, 1);

	buffer->Copy(output, sizeof(Output)* size);

	FILE* file;
	fopen_s(&file, "TextData/RawTest.csv", "w");

	for (UINT i = 0; i < size; i++)
	{
		Output temp = output[i];

		fprintf
		(
			file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
			i,
			temp.groupID[0], temp.groupID[1], temp.groupID[2],
			temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
			temp.dispatchThreadID[0], temp.dispatchThreadID[1], temp.dispatchThreadID[2],
			temp.groupIndex
		);
	}

	fclose(file);
}
