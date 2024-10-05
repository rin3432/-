#include "stdafx.h"
#include "Shadow.h"
#include "Game.h"

void InitRootSignature(RootSignature& rs);

Shadow::Shadow()
{

}

Shadow::~Shadow()
{

}

bool Shadow::Start()
{
	RootSignature rs;
	InitRootSignature(rs);

	lightCamera.SetPosition(0, 500, 0);
	lightCamera.SetTarget(0, 0, 0);
	lightCamera.SetUp(1, 0, 0);
	lightCamera.SetViewAngle(Math::DegToRad(20.0f));
	lightCamera.Update();

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	ModelInitData shadowModelInitData;

	shadowModelInitData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
	shadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	
	shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

	shadowModel.Init(shadowModelInitData);
	shadowModel.UpdateWorldMatrix(
		{ 0,50,0 },
		g_quatIdentity,
		g_vec3One
	);

	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &shadowMap.GetRenderTargetTexture();
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData.m_width = 256;
	spriteInitData.m_height = 256;

	
	sprite.Init(spriteInitData);

	m_light.SetEyePos(g_camera3D->GetPosition());
	m_model.UpdateWorldMatrix(
		{ 0, 50, 0 },
		g_quatIdentity,
		g_vec3One);

	ModelInitData initData;
	//スタンダードシェーダーを指定する。
	initData.m_fxFilePath = "Assets/shader/modelStandard.fx";
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	initData.m_expandConstantBuffer = &m_light;
	initData.m_expandConstantBufferSize = sizeof(m_light);

	m_model.Init(initData);

	ModelInitData bgModelInitData;
	bgModelInitData.m_fxFilePath = "Assets/shader/shadowReciever.fx";
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();

	bgModelInitData.m_expandConstantBuffer = 
		(void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = 
		sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/flat.tkm";

	bgModel.Init(bgModelInitData);
	
	return true;
}

bool Shadow::UpdateShadow()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	renderContext.SetRenderTargetAndViewport(shadowMap);
	renderContext.ClearRenderTargetView(shadowMap);

	shadowModel.Draw(renderContext, lightCamera);

	renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);

	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	//m_model.Draw(renderContext);

	sprite.Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });
	sprite.Draw(renderContext);

	bgModel.Draw(renderContext);

	return true;
}

void Shadow::Update()
{
	
}

void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}
