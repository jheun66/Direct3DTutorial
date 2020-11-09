struct VertexOutput
{
    float4 pos : POSITION;
};

VertexOutput VS(VertexOutput input)
{
    VertexOutput output;
    output.pos = input.pos;
   
    return output;
}