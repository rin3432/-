/*!
 * @brief 影が落とされるモデル用のシェーダー
 */

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer ShadowCb : register(b1)
{
    float4x4 mLVP;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;

    float4 posInLVP : TEXCOORD1;

};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t10);
sampler g_sampler : register(s0);

//影が落とされる3Dモデル用の頂点シェーダー
SPSIn VSMain(SVSIn vsIn)
{
    //通常の座標変換
    SPSIn psIn;
    float4 worldPos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, worldPos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(mWorld, vsIn.normal);

    psIn.posInLVP = mul(mLVP, worldPos);

    return psIn;
}


//影が落とされる3Dモデル用のピクセルシェーダー
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 color = g_albedo.Sample(g_sampler, psIn.uv);

    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
        if(zInLVP>zInShadowMap)
        {
            color.xyz *= 0.5f;
        }
    }
    
    

    return color;
}
