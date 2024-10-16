cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn
{
    float4 pos : POSITION; //���f���̒��_���W�B
    float3 normal : NORMAL;
    
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    
    float2 uv : TEXCOORD0; //UV���W�B
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; //uv���W�B
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2;
};
//�f�B���N�V�������C�g�p�̒萔�o�b�t�@�B
cbuffer DirectionLightCb : register(b1)
{
    float3 dirDirection;
    float3 dirColor;
    
    float3 ptPosition;
    float3 ptColor;
    float ptRange;
    
    float3 spPosition;
    float3 spColor;
    float spRange;
    float3 spDirection;
    float spAngle;
    
    float3 eyePos;
    float specPow;
    float3 ambientLight;
    
    float3 groundColor;
    float3 skyColor;
    float3 groundNormal;
}

float3 CalcNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);
float3 CalcLambertDiffuse(float3 normal);
float3 CalcPhongSpecular(float3 normal, float3 worldPos, float2 uv);


Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
Texture2D<float4> g_aoMap : register(t10);

sampler g_sampler : register(s0);

SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));

    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));

    psIn.uv = vsIn.uv;

    return psIn;
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    // �f�B�t���[�Y�}�b�v���T���v�����O
    float4 diffuseMap = g_texture.Sample(g_sampler, psIn.uv);
    // �@�����v�Z
    float3 normal =
        CalcNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

    // Lambert�g�U���ˌ����v�Z����
    float3 diffuseLig = CalcLambertDiffuse(normal);

    // Phong���ʔ��˂��v�Z
    float3 specLig = CalcPhongSpecular(normal, psIn.worldPos, psIn.uv);

    // �������v�Z
    float3 ambient = ambientLight;

    float ambientPower = g_aoMap.Sample(g_sampler, psIn.uv);
    ambient *= ambientPower;
    // �g�U���� + ���ʔ��� + ���������Z���čŏI�I�Ȕ��ˌ����v�Z����
    float3 lig = diffuseLig + specLig + ambient;

    float4 finalColor = diffuseMap;
    finalColor.xyz *= lig;

    return finalColor;
}

float3 CalcNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
    float3 newNormal = tangent * binSpaceNormal.x
                     + biNormal * binSpaceNormal.y
                     + normal * binSpaceNormal.z;
    return newNormal;
}

float3 CalcLambertDiffuse(float3 normal)
{
    return max(0.0f, dot(normal, -dirLigDirection)) * dirLigColor;
}

float3 CalcPhongSpecular(float3 normal, float3 worldPos, float2 uv)
{
    // ���˃x�N�g�������߂�
    float3 refVec = reflect(dirLigDirection, normal);

    // �������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // ���ʔ��˂̋��������߂�
    float t = saturate(dot(refVec, toEye));

    // ���ʔ��˂̋������i��
    t = pow(t, 5.0f);

    //  ���ʔ��ˌ������߂�
    float3 specularLig = dirLigColor * t;

    // �X�y�L�����}�b�v����X�y�L�������˂̋������T���v�����O
    float specPower = g_specularMap.Sample(g_sampler, uv).a;

    // ���ʔ��˂̋��������ʔ��ˌ��ɏ�Z����
    specularLig *= specPower * 2.0f;
    return specularLig;
}