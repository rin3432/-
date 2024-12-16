//

//頂点構造体。
struct SVertex
{
	float3 pos;
	float3 normal;
	float3 tangent;
	float3 tangent;
	float3 binormal;
	float2 uv;
	int4 indices;
	float4 skinweigths;
};

//引数構造体。
struct RayPayload
{
	float3 color;
	int hit;
};

RaytracingAccelerationStructure g_raytracingWorld : register(t0);
Texture2D<float4> g_albedoTexture : register(t1);
StructuredBuffer<SVertex> g_vertexBuffers : register(t6);
StructuredBuffer<int> g_indexBuffers : register(t7);

SamplerState g_samplerState : register(s0);

//関数宣言。
float2 GetUV(BuiltInTriangleIntersectionAttributes attribs);