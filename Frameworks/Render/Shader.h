#pragma once

class VertexShader;
class PixelShader;
class ComputeShader;
class GeometryShader;
class HullShader;
class DomainShader;

class Shader
{
private:
	static map<wstring, Shader*> totalShader;

public:
	static VertexShader* AddVS(wstring file, string vsName = "VS");
	static PixelShader* AddPS(wstring file, string psName = "PS");
	static ComputeShader* AddCS(wstring file, string csName = "CS");
	static GeometryShader* AddGS(wstring file, string gsName = "GS");
	static HullShader* AddHS(wstring file, string hsName = "HS");
	static DomainShader* AddDS(wstring file, string dsName = "DS");

	static void Delete();

	virtual void Set() = 0;
};