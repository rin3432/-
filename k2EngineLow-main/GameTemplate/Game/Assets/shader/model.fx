/*!
 * @brief	シンプルなモデルシェーダー。
 */


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float3 normal : NORMAL;
    
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    
    float2 uv : TEXCOORD0; //UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
	float2 uv 			: TEXCOORD0;	//uv座標。
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

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromDirectionLight(SPSIn psIn);
float3 CalcPhongSpecular(float3 normal, float3 worldPos);
////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
//Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
//StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

Texture2D<float4> g_aoMap : register(t10);
////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////


/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
       skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
       w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }

    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));

    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    psIn.normalInView = mul(mView, psIn.normal);

    psIn.uv = vsIn.uv;
    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
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
    psIn.normalInView = mul(mView, psIn.normal);

    psIn.uv = vsIn.uv;
    return psIn;
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

//float4 PSMain(SPSIn psIn) : SV_Target0
//{
//    // ディフューズマップをサンプリング
//    float4 diffuseMap = g_texture.Sample(g_sampler, psIn.uv);

//    float3 normal = psIn.normal;

//    // step-5 法線マップからタンジェントスペースの法線をサンプリングする
//    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    
//    localNormal = (localNormal - 0.5f) * 2.0f;
//    // step-6 タンジェントスペースの法線をワールドスペースに変換する
//    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;
//    // 拡散反射光を計算する
//    float3 lig = 0.0f;
//    lig += max(0.0f, dot(normal, -dirDirection)) * dirColor;
//    lig += ambientLight;

//    float4 finalColor = diffuseMap;
//    finalColor.xyz *= lig;

//    return finalColor;
//}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 diffuseMap = g_texture.Sample(g_sampler, psIn.uv);
    float3 normal = psIn.normal;
    
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    localNormal = (localNormal - 0.5f) * 2.0f;
    
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;
    
    //法線マップ。
    float3 normalLig = 0.0f;
    normalLig += max(0.0f, dot(normal, -dirDirection)) * dirColor;
    normalLig += ambientLight;
    
    //スペキュラマップ。
    float3 diffuseLig = max(0.0f, dot(normal, -dirDirection)) * dirColor;
    float specLig = CalcPhongSpecular(normal, psIn.worldPos) * 10.0f;
    float specPower = g_specularMap.Sample(g_sampler, psIn.uv).r;
    specLig *= specPower * 10.0f;
    
    //AOマップ。
    float3 ambient = ambientLight;
    float ambientPower = g_aoMap.Sample(g_sampler, psIn.uv);
    ambient *= ambientPower;
    
    
    float lig = normalLig + specLig + ambient;
    
    
    float3 directionLig = CalcLigFromDirectionLight(psIn);
    float3 pointLig = CalcLigFromPointLight(psIn);

    float3 ligDir = psIn.worldPos - spPosition;
    ligDir = normalize(ligDir);

    float3 diffSpotLight = CalcLambertDiffuse(
        ligDir, // ライトの方向
        spColor, // ライトのカラー
        psIn.normal // サーフェイスの法線
    );

    float3 specSpotLight = CalcPhongSpecular(
        ligDir, // ライトの方向
        spColor, // ライトのカラー
        psIn.worldPos, // サーフェイズのワールド座標
        psIn.normal     // サーフェイズの法線
    );

    // ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - spPosition);

    // 影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / spRange * distance;

    // 影響力がマイナスにならないように補正をかける
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    affect = pow(affect, 3.0f);

    diffSpotLight *= affect;
    specSpotLight *= affect;
    
    float angle = dot(ligDir, spDirection);
    
    angle = abs(acos(angle));
    affect = 1.0f - 1.0f / spAngle * angle;
    
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    affect = pow(affect, 0.5f);
    
    diffSpotLight *= affect;
    specSpotLight *= affect;
    
    //ここからリムライト計算。
    float power1 = 1.0f - max(0.0f, dot(dirDirection, psIn.normal));
    float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
    float limPower = power1 * power2;
    
    limPower = pow(limPower, 1.3f);
    
    float3 limColor = limPower * dirColor;
    
    //ここから半球ライト計算。
    float t = dot(psIn.normal, groundNormal);
    t = (t + 1.0f) / 2.0f;
    
    float3 hemiLight = lerp(groundColor, skyColor, t);
    
    float3 finalLig = directionLig + pointLig + ambientLight;
    
    
    
    
    //limColorがおかしい
    finalLig = finalLig + diffSpotLight + specSpotLight + limColor + hemiLight;
    
    //finalLig *= lig;
    
    

    float4 finalColor = diffuseMap;
    

    // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= finalLig;
    finalColor.xyz *= lig;
    
    return finalColor;
}

//SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
//{
//    SPSIn psIn;

//    psIn.pos = mul(mWorld, vsIn.pos); // モデルの頂点をワールド座標系に変換
//    psIn.worldPos = psIn.pos;
//    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
//    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換

//    psIn.normal = mul(mWorld, vsIn.normal); // 法線を回転させる
//    psIn.uv = vsIn.uv;

//    psIn.normalInView = mul(mView, psIn.normal);
//    return psIn;
//}

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    float t = dot(normal, lightDirection) * -1.0f;

    t = max(0.0f, t);

    return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    float3 refVec = reflect(lightDirection, normal);

    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    float t = dot(refVec, toEye);

    t = max(0.0f, t);

    t = pow(t, 5.0f);

    return lightColor * t;
}

/// ポイントライトによる反射光を計算
float3 CalcLigFromPointLight(SPSIn psIn)
{
    float3 ligDir = psIn.worldPos - ptPosition;
    ligDir = normalize(ligDir);
    
    float3 diffPoint = CalcLambertDiffuse(
        ligDir,
        ptColor,
        psIn.normal
    );

    float3 specPoint = CalcPhongSpecular(
        ligDir,
        ptColor,
        psIn.worldPos,
        psIn.normal
    );
    
    float3 distance = length(psIn.worldPos - ptPosition);
    
    float affect = 1.0f - 1.0f / ptRange * distance;
    
    
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    affect = pow(affect, 3.0f);
    
    diffPoint *= affect;
    specPoint *= affect;
    
    return diffPoint + specPoint;
}

/// ディレクションライトによる反射光を計算
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

    float3 specDirection = CalcPhongSpecular(
            dirDirection, dirColor, psIn.worldPos, psIn.normal);
    return diffDirection + specDirection;
}

float3 CalcPhongSpecular(float3 normal, float3 worldPos)
{
    float3 refVec = reflect(dirDirection, normal);

    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    float t = saturate(dot(refVec, toEye));

    t = pow(t, 5.0f);

    float3 specularLig = dirColor * t;
    return specularLig;
}
