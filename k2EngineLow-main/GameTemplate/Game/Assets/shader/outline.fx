///////////////////////////////////////////
// �\����
///////////////////////////////////////////
// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
};

///////////////////////////////////////////
// �萔�o�b�t�@�[
///////////////////////////////////////////
// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

///////////////////////////////////////////
// �V�F�[�_�[���\�[�X
///////////////////////////////////////////
// ���f���e�N�X�`��
Texture2D<float4> g_texture : register(t0);

// step-3 �[�x�e�N�X�`���ɃA�N�Z�X���邽�߂̕ϐ���ǉ�
Texture2D<float4> g_depthTexture : register(t10);

///////////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////////
sampler g_sampler : register(s0);

/// <summary>
/// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.uv = vsIn.uv;

    //step-4 ���_�̐��K���X�N���[�����W�n�̍��W���s�N�Z���V�F�[�_�[�ɓn��
    psIn.posInProj = psIn.pos;
    psIn.posInProj.xy /= psIn.posInProj.w;

    return psIn;
}

/// <summary>
/// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // step-5 �ߖT8�e�N�Z���̐[�x�l���v�Z���āA�G�b�W�𒊏o����
    // ���K���X�N���[�����W�n����UV���W�n�ɕϊ�����
    float2 uv = psIn.posInProj.xy * float2(0.5f, -0.5f) + 0.5f;

    // �ߖT8�e�N�Z���ւ�UV�I�t�Z�b�g
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 720.0f), //��
        float2(0.0f, -1.0f / 720.0f), //��
        float2(1.0f / 1280.0f, 0.0f), //�E
        float2(-1.0f / 1280.0f, 0.0f), //��
        float2(1.0f / 1280.0f, 1.0f / 720.0f), //�E��
        float2(-1.0f / 1280.0f, 1.0f / 720.0f), //����
        float2(1.0f / 1280.0f, -1.0f / 720.0f), //�E��
        float2(-1.0f / 1280.0f, -1.0f / 720.0f) //����
    };

    // ���̃s�N�Z���̐[�x�l���擾
    float depth = g_depthTexture.Sample(g_sampler, uv).x;

    // �ߖT8�e�N�Z���̐[�x�l�̕��ϒl���v�Z����
    float depth2 = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_depthTexture.Sample(g_sampler, uv + uvOffset[i]).x;
    }
    depth2 /= 8.0f;

    // ���g�̐[�x�l�ƋߖT8�e�N�Z���̐[�x�l�̍��𒲂ׂ�
    if (abs(depth - depth2) > 0.00005f)
    {
        // �[�x�l�����\�Ⴄ�ꍇ�̓s�N�Z���J���[�����ɂ���
        // ->���ꂪ�G�b�W�J���[�ƂȂ�
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // ���ʂɃe�N�X�`����
    return g_texture.Sample(g_sampler, psIn.uv);
}
