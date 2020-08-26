#pragma once

class VertexShader;
class PixelShader;
class ComputeShader;

class Shader
{
private:
	static map<wstring, Shader*> totalShader;

public:
	static VertexShader* AddVS(wstring file, string vsName = "VS");
	static PixelShader* AddPS(wstring file, string psName = "PS");
	static ComputeShader* AddCS(wstring file, string csName = "CS");

	static void Delete();

	virtual void Set() = 0;
};