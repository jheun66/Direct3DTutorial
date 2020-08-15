#include "Framework.h"

map<wstring, Texture*> Texture::totalTexture;

// move => 이동생성자
Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
    :srv(srv), image(move(image))
{

}

Texture::~Texture()
{
    srv->Release();
}

Texture* Texture::Add(wstring file)
{
    if (totalTexture.count(file) > 0)
        return totalTexture[file];

    wstring extension = Utility::GetExtension(file);

    ScratchImage image;

    wstring path = L"Textures/" + file;

    if (extension == L"tga")
    {
        V(LoadFromTGAFile(path.c_str(), nullptr, image));
    }
    else if (extension == L"dds")
    {
        V(LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image));
    }
    else
    {
        V(LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image));
    }

    ID3D11ShaderResourceView* srv;

    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    /*
    * 맵에 넣는 방법
    totalTexture[file] = new Texture(srv, image);
    totalTexture.insert(pair<wstring, Texture*>(file, new Texture(srv, image)));
    totalTexture.insert(make_pair(file, new Texture(srv, image)));
    totalTexture.insert({ file, new Texture(srv, image) });
    */
    totalTexture[file] = new Texture(srv, image);

    return totalTexture[file];
}

void Texture::Delete()
{
    for (auto texture : totalTexture)
    {
        delete texture.second;
    }

}

void Texture::PSSet(UINT slot)
{
    DC->PSSetShaderResources(slot, 1, &srv);
}

vector<Float4> Texture::ReadPixels()
{
    vector<Float4> result;

    uint8_t* colors = image.GetPixels();
    UINT size = image.GetPixelsSize();

    float scale = 1.0f / 255.0f;

    for (int i = 0; i < size; i += 4)
    {
        Float4 color;

        color.x = colors[i + 0] * scale;
        color.y = colors[i + 1] * scale;
        color.z = colors[i + 2] * scale;
        color.w = colors[i + 3] * scale;

        result.emplace_back(color);
    }
    return result;
}
