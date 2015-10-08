cbuffer PS_Constant_Buffer : register(b0)
{
    float4 color;
};

float4 PS_Main (in float4 pos : SV_POSITION) : SV_TARGET
{
    return color;
}