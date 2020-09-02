cbuffer W : register(b0)
{
    matrix world;
}

cbuffer V : register(b1)
{
    matrix view;
    matrix invView;
}
cbuffer P : register(b2)
{
    matrix projection;
}

cbuffer ModelBone : register(b3)
{
    // MAX_MODEL_BONE 개수 맞추기
    matrix bones[256];
    
    int index;
}