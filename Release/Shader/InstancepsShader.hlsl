cbuffer PS_Color : register(b0)
{
	float4 Color;
}

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

//��������
//���÷��� ��� �ȼ��� ó���Ǵ���
SamplerState Sampler : register(s0);
//�ؽ��� �ڿ�
Texture2D Texture : register(t0);

//��� �����
float4 InstancePixelShader(PixelInput input) : SV_TARGET
{
    float4 color = Texture.Sample(Sampler, input.Uv);

    if (color.r > 0.9f && color.g < 0.1f && color.b > 0.9f)
    {
        color= 0.0f;
    }
	if (any(color))
	{
		//�Ǽ� ������ ���ϸ� ���Ҽ��� ��ο� ����
		//color = color * Color;
		//��ο� ���� ������ �ݴ밪 ��� ���Ҽ��� �����
        color.r = (1.0f - ((1.0f - color.r) * (1.0f - input.Color.r)));
        color.g = (1.0f - ((1.0f - color.g) * (1.0f - input.Color.g)));
        color.b = (1.0f - ((1.0f - color.b) * (1.0f - input.Color.b)));
        color.a = color.a * input.Color.a;
    }

    return color;
}
