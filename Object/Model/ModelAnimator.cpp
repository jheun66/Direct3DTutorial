#include "Framework.h"

ModelAnimator::ModelAnimator(string file)
	:texture(nullptr), srv(nullptr), frustumCount(0)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModelAnimationInstancing", L"PixelNormalMapping");

	frameBuffer = new FrameBuffer();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);

	typeBuffer->data.option[0] = 1;

	frustum = new Frustum();
}

ModelAnimator::~ModelAnimator()
{
	delete frameBuffer;
	delete[] clipTransform;
	delete[] nodeTransform;

	texture->Release();
	srv->Release();

	for (ModelClip* clip : clips)
		delete clip;

	for (Transform* transform : transforms)
		delete transform;

	delete instanceBuffer;
}

void ModelAnimator::Update()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		TweenDesc& tweenDesc = frameBuffer->data.tweenDesc[i];


		{// ���� �ִϸ��̼�
			KeyFrameDesc& desc = tweenDesc.cur;
			ModelClip* clip = clips[desc.clip];

			float time = 1.0f / clip->frameRate / desc.speed;
			desc.runningTime += DELTA;

			if (desc.time >= 1.0f)	// ���� ���������� �ѱ�� ����
			{
				// ���� �ִϸ��̼��� ������ ���� (������ ���� ����)
				if (desc.curFrame + desc.time >= clip->frameCount)
				{
					if (EndEvent.count(desc.clip) > 0)
						EndEvent[desc.clip]();
				}

				desc.runningTime = 0.0f;

				//									���� ����
				desc.curFrame = (desc.curFrame + 1) % clip->frameCount;
				desc.nextFrame = (desc.curFrame + 1) % clip->frameCount;
			}

			desc.time = desc.runningTime / time;
		}

		{// ���� �ִϸ��̼�
			KeyFrameDesc& desc = tweenDesc.next;

			if (desc.clip > -1)
			{
				ModelClip* clip = clips[desc.clip];

				tweenDesc.runningTime += DELTA;
				tweenDesc.tweenTime = tweenDesc.runningTime / tweenDesc.takeTime;

				if (tweenDesc.tweenTime >= 1.0f)
				{
					tweenDesc.cur = desc;
					tweenDesc.runningTime = 0.0f;
					tweenDesc.tweenTime = 0.0f;

					desc.runningTime = 0.0f;
					desc.curFrame = 0;
					desc.nextFrame = 0;
					desc.time = 0.0f;
					desc.clip = -1;
				}
				else
				{
					float time = 1.0f / clip->frameRate / desc.speed;
					desc.runningTime += DELTA;

					if (desc.time >= 1.0f)
					{
						desc.runningTime = 0.0f;

						desc.curFrame = (desc.curFrame + 1) % clip->frameCount;

						desc.nextFrame = (desc.curFrame + 1) % clip->frameCount;
					}
					desc.time = desc.runningTime / time;
				}
			}
		}
	}

	UpdateTransforms();
}

void ModelAnimator::Render()
{
	if (texture == nullptr)
		CreateTexture();

	instanceBuffer->IASet(1);

	// �������� 4��
	frameBuffer->SetVSBuffer(4);
	typeBuffer->SetVSBuffer(5);

	// texture�� 0��
	DC->VSSetShaderResources(0, 1, &srv);

	for (ModelMesh* mesh : meshes)
		mesh->Render(frustumCount);
}

void ModelAnimator::PostRender()
{
	for (Transform* transform : transforms)
		transform->PostRender();

	ImGui::Text("DrawCount : %d", frustumCount);
}

void ModelAnimator::UpdateTransforms()
{
	frustum->Update();

	frustumCount = 0;
	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (frustum->ContainPoint(transforms[i]->WorldPos()))
		{
			transforms[i]->isActive = true;
			transforms[i]->UpdateWorld();
			Matrix temp = XMMatrixTranspose(*transforms[i]->GetWorld());
			memcpy(&worlds[frustumCount++], &temp, sizeof(Matrix));
		}
		else
		{
			transforms[i]->isActive = false;
		}
	}

	instanceBuffer->Update(worlds, MAX_MODEL_INSTANCE);
}

Transform* ModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.emplace_back(transform);

	return transform;
}

void ModelAnimator::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
	frameBuffer->data.tweenDesc[instance].takeTime = takeTime;
	frameBuffer->data.tweenDesc[instance].next.clip = clip;
	frameBuffer->data.tweenDesc[instance].next.speed = speed;
}

void ModelAnimator::ReadClip(string file)
{
	file = "ModelData/Clips/" + file + ".clip";

	BinaryReader* r = new BinaryReader(ToWString(file));

	ModelClip* clip = new ModelClip();

	clip->name = r->String();
	clip->duration = r->Float();
	clip->frameRate = r->Float();
	clip->frameCount = r->UInt();

	UINT keyFrameCount = r->UInt();
	for (UINT i = 0; i < keyFrameCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r->Byte(&ptr, sizeof(KeyTransform) * size);
		}
		clip->keyFrameMap[keyFrame->boneName] = keyFrame;
	}
	clips.emplace_back(clip);

	delete r;
}

Matrix ModelAnimator::GetCurNodeMatrix(UINT nodeIndex)
{
	if (nodeTransform == nullptr)
		return XMMatrixIdentity();

	UINT curClip = frameBuffer->data.tweenDesc[0].cur.clip;
	UINT curFrame = frameBuffer->data.tweenDesc[0].cur.curFrame;

	return nodeTransform[curClip].transform[curFrame][nodeIndex];
}

void ModelAnimator::CreateTexture()
{
	UINT clipCount = clips.size();

	clipTransform = new ClipTransform[clipCount];
	nodeTransform = new ClipTransform[clipCount];

	for (UINT i = 0 ; i < clipCount; i++)
		CreateClipTransform(i);
	
	{//Craete Texture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = MAX_MODEL_BONE * 4;	// �ؽ�ó�� ���� ����, x4�� rgba
		desc.Height = MAX_ANIM_KEY;
		desc.ArraySize = clipCount;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		// �츮�� ���� �޸� ũ��
		UINT pageSize = MAX_MODEL_BONE * sizeof(Float4x4) * MAX_ANIM_KEY;

		// malloc�� ũ�⸦ �������, �뷮�� ū�Ŵ� ����޸� ����
		void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < clipCount; c++)
		{
			UINT start = c * pageSize;

			for (UINT y = 0; y < MAX_ANIM_KEY; y++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_BONE * y * sizeof(Matrix) + start;

				// ���� �޸� �ɰ��� �Ҵ�
				VirtualAlloc(temp, MAX_MODEL_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				// memcpy�� �ɰ��� �־���
				memcpy(temp, clipTransform[c].transform[y], MAX_MODEL_BONE * sizeof(Matrix));
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];
		for (UINT c = 0; c < clipCount; c++)
		{
			void* temp = (BYTE*)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_BONE * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		V(DEVICE->CreateTexture2D(&desc, subResource, &texture));
		delete[] subResource;
		VirtualFree(p, 0, MEM_RELEASE);
	}


	{// Create SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = clipCount;

		V(DEVICE->CreateShaderResourceView(texture, &desc, &srv));
	}
	
}

// texture�� �־��ټ� �ֵ���
void ModelAnimator::CreateClipTransform(UINT index)
{
	ModelClip* clip = clips[index];
	for (UINT f = 0; f < clip->frameCount; f++)
	{
		UINT nodeIndex = 0;
		for (NodeData* node : nodes)
		{
			Matrix animation;
			KeyFrame* frame = clip->GetKeyFrame(node->name);
			if (frame != nullptr)
			{
				KeyTransform& transform = frame->transforms[f];

				Matrix S = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
				Matrix R = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.rotation));
				Matrix T = XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

				animation = S * R * T;
			}
			else
			{
				animation = XMMatrixIdentity();
			}

			Matrix parent;
			int parentIndex = node->parent;
			if (parentIndex < 0)
				parent = XMMatrixIdentity();
			else
				parent = nodeTransform[index].transform[f][parentIndex];

			nodeTransform[index].transform[f][nodeIndex] = animation * parent;

			if (boneMap.count(node->name) > 0)
			{
				int boneIndex = boneMap[node->name];
				Matrix offset = XMLoadFloat4x4(&bones[boneIndex]->offset);
				offset *= nodeTransform[index].transform[f][nodeIndex];
				clipTransform[index].transform[f][boneIndex] = offset;
			}

			nodeIndex++;
		}
	}
}