static const float PI = 3.1415926f;

// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

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

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION; // ���f���̒��_���W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // UV���W
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // uv���W
    float3 worldPos : TEXCOORD1; // ���[���h��Ԃł̃s�N�Z���̍��W
};

Texture2D<float4> g_albedo : register(t0); // �A���x�h�}�b�v
Texture2D<float4> g_normalMap : register(t1); // �@���}�b�v
Texture2D<float4> g_specularMap : register(t2); // �X�y�L�����}�b�v�Brgb�ɃX�y�L�����J���[�Aa�ɋ����x

// �T���v���[�X�e�[�g
sampler g_sampler : register(s0);

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

//�N�b�N�g�����X���f���̋��ʔ��˂��v�Z
float CookTrranceSpecular(float3 L, float3 V, float3 N, float metaric)
{
    float microfacet = 0.76f;

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = metaric;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = Beckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = SpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = PI * NdotV * NdotH;

    // �����܂ŋ��߂��A�l�𗘗p���āA�N�b�N�g�����X���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-4 �t���l�����˂��l�������g�U���ˌ������߂�
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // 3. �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotNL = saturate(dot(N, L));

    float dotNV = saturate(dot(N, V));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗�����Z���āA
    // �ŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (dotNL * dotNV) / PI;
}

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
    // �@�����v�Z
    float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

    // step-2 �A���x�h�J���[�A�X�y�L�����J���[�A�����x���T���v�����O����
    // �A���x�h�J���[�i�g�U���ˌ��j
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

    // �X�y�L�����J���[�i���ʔ��ˌ��j
    float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;

    // �����x
    float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;

    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
    float3 toEye = normalize(eyePos - psIn.worldPos);

    // step-3 �f�B�Y�j�[�x�[�X�̊g�U���˂���������

    // �t���l�����˂��l�������g�U���˂��v�Z
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -dirDirection, toEye);

    // ���K��Lambert�g�U���˂����߂�
    float NdotL = saturate(dot(normal, -dirDirection));
    float3 lambertDiffuse = dirColor * NdotL;

    // �ŏI�I�Ȋg�U���ˌ����v�Z����
   float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;


    // �N�b�N�g�����X���f���̋��ʔ��˗����v�Z����
    float3 spec = CookTrranceSpecular(-dirDirection, toEye, normal, metaric) * dirColor;

    // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
    // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
    float specTerm = length(specColor.xyz);
     spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

    float3 lig = 0;
    // ���ʔ��˗���������΁A�g�U���˂͎キ�Ȃ�
    lig += diffuse * (1.0f - specTerm) + spec;

    // �����ɂ���グ
    lig += ambientLight * albedoColor;
    
    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    return finalColor;
}