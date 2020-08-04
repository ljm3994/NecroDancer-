cbuffer VS_Vp : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}
cbuffer VS_FrameUv : register(b2)
{
    float4 FrameUv;
}

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
	float4 OneMtx : TEXCOORD1;
    float4 TwoMtx : TEXCOORD2;
    float4 ThreeMtx : TEXCOORD3;
    float4 FourMtx : TEXCOORD4;
    float4 Color : TEXCOORD5;
	float4 InstanceUV : TEXCOORD6;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float2 Uv : TEXCOORD0;
    float4 OneMtx : TEXCOORD1;
    float4 TwoMtx : TEXCOORD2;
    float4 ThreeMtx : TEXCOORD3;
    float4 FourMtx : TEXCOORD4;
    float4 InstanceUV : TEXCOORD5;
    float4 Color : TEXCOORD6;
};

PixelInput InstanceVertexShader(VertexInput input)
{
    PixelInput output;
    matrix Temp;

    Temp._11 = input.OneMtx.x;
    Temp._12 = input.OneMtx.y;
    Temp._13 = input.OneMtx.z;
    Temp._14 = input.OneMtx.w;
    Temp._21 = input.TwoMtx.x;
    Temp._22 = input.TwoMtx.y;
    Temp._23 = input.TwoMtx.z;
    Temp._24 = input.TwoMtx.w;
    Temp._31 = input.ThreeMtx.x;
    Temp._32 = input.ThreeMtx.y;
    Temp._33 = input.ThreeMtx.z;
    Temp._34 = input.ThreeMtx.w;
    Temp._41 = input.FourMtx.x;
    Temp._42 = input.FourMtx.y;
    Temp._43 = input.FourMtx.z;
    Temp._44 = input.FourMtx.w;

    output.Position = mul(input.Position, Temp);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

	output.Uv = input.Uv;

	if (output.Uv.x == 0.0f)
	{
		output.Uv.x = input.InstanceUV.x;
	}
	if (output.Uv.x == 1.0f)
	{
		output.Uv.x = input.InstanceUV.z;
	}
	if (output.Uv.y == 0.0f)
	{
		output.Uv.y = input.InstanceUV.y;
	}
	if (output.Uv.y == 1.0f)
	{
		output.Uv.y = input.InstanceUV.w;
	}
    output.Color = input.Color;
    return output;
}