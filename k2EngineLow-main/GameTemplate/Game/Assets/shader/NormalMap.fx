cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float3 normal : NORMAL;
    
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    
    float2 uv : TEXCOORD0; //UV座標。
    SSkinVSIn skinVert; //スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; //uv座標。
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2;
};
//ディレクションライト用の定数バッファ。
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

sampler g_sampler : register(s0); //サンプラステート。

Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);

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
    float4 diffuseMap = g_texture.Sample(g_sampler, psIn.uv);

    float3 normal = psIn.normal;

    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    
    localNormal = (localNormal - 0.5f) * 2.0f;
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;
    // 拡散反射光を計算する
    float3 lig = 0.0f;
    lig += max(0.0f, dot(normal, -dirDirection)) * dirColor;
    lig += ambientLight;

    float4 finalColor = diffuseMap;
    finalColor.xyz *= lig;

    return finalColor;
}