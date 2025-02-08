/*!
 * @brief ���j�A���C�v
 */

cbuffer cb : register(b0)
{
    float4x4 mvp; // MVP�s��
    float4 mulColor; // ��Z�J���[
};

// step-4 ���C�v�T�C�Y�ɃA�N�Z�X���邽�߂̒萔�o�b�t�@�[���`����

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0); // �J���[�e�N�X�`��
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
    float4 color = colorTexture.Sample(Sampler, In.uv);

    // step-5 ���C�v�T�C�Y�̒l�ɉ����ăs�N�Z�����N���b�v����

    return color;
}
