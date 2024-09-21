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
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float2 uv 		: TEXCOORD0;	//UV座標。
    float3 normal : NORMAL;
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL;
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
    float3 ambientLight; 
    
    float3 groundColor;
    float3 skyColor;
    float3 groundNormal;
}

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromDirectionLight(SPSIn psIn);
////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t2);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

Texture2D<float4> g_texture : register(t0);
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
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    psIn.normal = mul(mWorld, vsIn.normal);
    psIn.uv = vsIn.uv;

    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
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
    
    finalLig += diffSpotLight + specSpotLight + limColor + hemiLight;

    float4 finalColor = g_texture.Sample(g_sampler, psIn.uv);

    // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= finalLig;

    return finalColor;
}

SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos); // モデルの頂点をワールド座標系に変換
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換

    psIn.normal = mul(mWorld, vsIn.normal); // 法線を回転させる
    psIn.uv = vsIn.uv;

    psIn.normalInView = mul(mView, psIn.normal);
    return psIn;
}

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