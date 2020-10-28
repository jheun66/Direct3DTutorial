#include "Framework.h"

DirectWrite* DirectWrite::instance = nullptr;

DirectWrite::DirectWrite()
{
	V(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory), (IUnknown**)&writeFactory));

	V(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory));

	IDXGIDevice* dxgiDevice;
	V(DEVICE->QueryInterface(&dxgiDevice));

	V(factory->CreateDevice(dxgiDevice, &device));

	V(device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, 
		&context));

	IDXGISurface* dxgiSurface;
	V(Device::Get()->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface),
		(void**)&dxgiSurface));

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	V(context->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap));

	context->SetTarget(targetBitmap);
}

DirectWrite::~DirectWrite()
{
	for (FontBrushDesc desc : fontBrush)
		desc.brush->Release();

	for (FontTextDesc desc : fontText)
		desc.format->Release();

	factory->Release();
	writeFactory->Release();
	targetBitmap->Release();
	context->Release();
	device->Release();

}

void DirectWrite::RenderText(wstring text, RECT rect, float size, wstring font, Float4 color, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	FontBrushDesc brushDesc;
	brushDesc.color = color;

	FontBrushDesc* findBrush = nullptr;
	for (FontBrushDesc& desc : fontBrush)
	{
		if (desc == brushDesc)
		{
			findBrush = &desc;
			break;
		}
	}

	if (findBrush == nullptr)
	{
		D2D1::ColorF colorF = D2D1::ColorF(color.x, color.y, color.z);
		context->CreateSolidColorBrush(colorF, &brushDesc.brush);

		fontBrush.emplace_back(brushDesc);
		findBrush = &brushDesc;
	}

	FontTextDesc textDesc;
	textDesc.font = font;
	textDesc.fontSize = size;
	textDesc.weight = weight;
	textDesc.style = style;
	textDesc.stretch = stretch;

	FontTextDesc* findText = nullptr;
	for (FontTextDesc& desc : fontText)
	{
		if (desc == textDesc)
		{
			findText = &desc;
			break;
		}
	}

	if (findText == nullptr)
	{
		writeFactory->CreateTextFormat(textDesc.font.c_str(),
			nullptr, textDesc.weight, textDesc.style,
			textDesc.stretch, textDesc.fontSize, L"ko", &textDesc.format);

		fontText.emplace_back(textDesc);
		findText = &textDesc;
	}

	D2D1_RECT_F rectF;
	rectF.left = (float)rect.left;
	rectF.right = (float)rect.right;
	rectF.top = (float)rect.top;
	rectF.bottom = (float)rect.bottom;

	context->DrawTextW(text.c_str(), text.size(), findText->format,
		&rectF, findBrush->brush);

}
