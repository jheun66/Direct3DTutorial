#pragma once

class VertexShader;
class PixelShader;


class Shader
{
private:
	static map<wstring, Shader*> totalShader;

public:
	static VertexShader* AddVS(wstring file, string vsName = "VS");
	static PixelShader* AddPS(wstring file, string psName = "PS");
	static void Delete();

	virtual void Set() = 0;
};