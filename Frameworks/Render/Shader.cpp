#include "Framework.h"

map<wstring, Shader*> Shader::totalShader;


VertexShader* Shader::AddVS(wstring file, string vsName)
{
    if (totalShader.count(file) > 0)
        return (VertexShader*)totalShader[file];

    totalShader[file] = new VertexShader(file, vsName);

    return (VertexShader*)totalShader[file];
}

PixelShader* Shader::AddPS(wstring file, string psName)
{
    if (totalShader.count(file) > 0)
        return (PixelShader*)totalShader[file];

    totalShader[file] = new PixelShader(file, psName);

    return (PixelShader*)totalShader[file];
}

ComputeShader* Shader::AddCS(wstring file, string csName)
{
    if (totalShader.count(file) > 0)
        return (ComputeShader*)totalShader[file];

    totalShader[file] = new ComputeShader(file, csName);

    return (ComputeShader*)totalShader[file];
}

void Shader::Delete()
{
    for (auto shader : totalShader)
        delete shader.second;
}
