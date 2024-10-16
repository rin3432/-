cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

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
    // ディフューズマップをサンプリング
    float4 diffuseMap = g_texture.Sample(g_sampler, psIn.uv);
    // 法線を計算
    float3 normal =
        CalcNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

    // Lambert拡散反射光を計算する
    float3 diffuseLig = CalcLambertDiffuse(normal);

    // Phong鏡面反射を計算
    float3 specLig = CalcPhongSpecular(normal, psIn.worldPos, psIn.uv);

    // 環境光を計算
    float3 ambient = ambientLight;

    float ambientPower = g_aoMap.Sample(g_sampler, psIn.uv);
    ambient *= ambientPower;
    // 拡散反射 + 鏡面反射 + 環境光を合算して最終的な反射光を計算する
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
    // 反射ベクトルを求める
    float3 refVec = reflect(dirLigDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = saturate(dot(refVec, toEye));

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);

    //  鏡面反射光を求める
    float3 specularLig = dirLigColor * t;

    // スペキュラマップからスペキュラ反射の強さをサンプリング
    float specPower = g_specularMap.Sample(g_sampler, uv).a;

    // 鏡面反射の強さを鏡面反射光に乗算する
    specularLig *= specPower * 2.0f;
    return specularLig;
}