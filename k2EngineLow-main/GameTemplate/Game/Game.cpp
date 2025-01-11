#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Shadow.h"
#include "Bullet.h"
#include "Turret.h"
#include "Lever.h"

const int NUM_WEIGHTS = 8;
/// <summary>
/// ブラー用のパラメーター
/// </summary>
struct SBlurParam
{
	float weights[NUM_WEIGHTS];
};

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	player = NewGO<Player>(0, "player");
	gameCamera = NewGO<GameCamera>(0, "gameCamera");
	backGround = NewGO<BackGround>(0, "backGround");
	shadow = NewGO<Shadow>(0,"shadow");
	bullet = NewGO<Bullet>(0, "bullet");
	turret = NewGO<Turret>(0, "turret");
	level = NewGO<Lever>(0, "level");

	InitDirLight();
	//InitPtLight();
	InitAmbLight();
	//InitSpotLight();
	
	//light.SetGroundColor(0.7f, 0.5f, 0.3f);
	//light.SetSkyColor(0.15f, 0.7f, 0.95f);
	//light.SetGroundNormal(0.3f, 0.3f, 0.3f);
	//light.SetSpecPow(5.0f);
	
	
	InitModel(bgModel, charaModel, lightModel, light);
	light.SetLight(directionLight, pointLight, spotLight);
	player->Init(light);
	backGround->Init(light);
	

	

	shadow->Start();

	return true;
}

void Game::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//SpotLight();
	light.SetLight(directionLight, pointLight, spotLight);

	shadow->UpdateShadow();

	//RenderTarget* rtArray[] = { &offscreenRenderTarget };
	//renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
	//renderContext.SetRenderTargets(1, rtArray);
	//renderContext.ClearRenderTargetViews(1, rtArray);

	////bgModel.Draw(renderContext);
	//charaModel.Draw(renderContext);
	////lightModel.Draw(renderContext);

	//renderContext.WaitUntilFinishDrawingToRenderTargets(1, rtArray);

	//renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//renderContext.SetRenderTargetAndViewport(mainRenderTarget);
	//renderContext.ClearRenderTargetView(mainRenderTarget);

	//charaModel.Draw(renderContext);
	//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	//renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
	//renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);
	//renderContext.ClearRenderTargetView(luminnceRenderTarget);
	//luminanceSprite.Draw(renderContext);
	//renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);

	//gaussianBlur.ExecuteOnGPU(renderContext, 20);

	//renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//renderContext.SetRenderTargetAndViewport(mainRenderTarget);
	//finalSprite.Draw(renderContext);
	//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	//renderContext.SetRenderTarget(
	//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
	//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	//);
	//copyToFrameBufferSprite.Draw(renderContext);

	boxModel.Draw(renderContext);
}

void Game::InitModel(Model& bgModel, Model& teapotModel, Model& lightModel, Light& light)
{

	RootSignature rs;
	InitRootSignature(rs);

	offscreenRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	ModelInitData boxModelInitData;
	boxModelInitData.m_tkmFilePath = "Assets/modelData/box.tkm";
	boxModelInitData.m_fxFilePath = "Assets/shader/offscreenRender.fx";
	
	boxModel.Init(boxModelInitData);
	boxModel.UpdateWorldMatrix({ 100.0f,0.0f,0.0f }, g_quatIdentity, g_vec3One);

	boxModel.ChangeAlbedoMap(
		"",
		offscreenRenderTarget.GetRenderTargetTexture()
	);

	
	luminnceRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	SpriteInitData luminanceSpriteInitData;
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	luminanceSpriteInitData.m_width = 1280;
	luminanceSpriteInitData.m_height = 720;
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32_FLOAT;

	
	luminanceSprite.Init(luminanceSpriteInitData);

	
	gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());
	
	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	finalSpriteInitData.m_width = 1280;
	finalSpriteInitData.m_height = 720;
	finalSpriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	
	finalSprite.Init(finalSpriteInitData);

	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;
	spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
	
	copyToFrameBufferSprite.Init(spriteInitData);


	ModelInitData lightModelInitData;
	lightModelInitData.m_tkmFilePath = "Assets/modelData/light.tkm";

	// 使用するシェーダーファイルパスを設定する
	lightModelInitData.m_fxFilePath = "Assets/shader/light.fx";
	lightModelInitData.m_expandConstantBuffer = &light;
	lightModelInitData.m_expandConstantBufferSize = sizeof(light);

	// 初期化情報を使ってモデルを初期化する
	lightModel.Init(lightModelInitData);
}

void Game::InitDirLight()
{
	directionLight.SetDirection(1.0f, 0.0f, 0.0f);
	//directionLight.NormalizeDirection();
	directionLight.SetColor(0.8f, 0.8f, 0.8f);
	//directionLight.SetColor(40.8f, 40.8f, 40.8f);

	light.SetEyePos(g_camera3D->GetPosition());
}

void Game::InitPtLight()
{
	pointLight.SetPosition(0.0f, 50.0f, 50.0f);
	pointLight.SetColor(0.0f, 0.0f, 0.0f);
	pointLight.SetRange(100.0f);
}

void Game::InitAmbLight()
{
	light.SetAmbient(0.5f, 0.5f, 0.5f);
}

void Game::InitSpotLight()
{
	spotLight.SetPosition(0.0f, 50.0f, 0.0f);
	spotLight.SetColor(10.0f, 10.0f, 10.0f);
	spotLight.SetDirection(1.0f, -1.0f, 1.0f);

	spotLight.NormalizeDirection();
	spotLight.SetRange(300.0f);
	spotLight.SetAngle(Math::DegToRad(25.0f));
}

void Game::SpotLight()
{
	Vector3 ligPos = spotLight.GetPosition();

	spotLight.SetPosition(ligPos.x + g_pad[0]->GetLStickXF(), ligPos.y, ligPos.z + g_pad[0]->GetLStickYF());
	ligPos = spotLight.GetPosition();
	if (g_pad[0]->IsPress(enButtonB))
	{
		spotLight.SetPosition(ligPos.x, ligPos.y + g_pad[0]->GetLStickYF(), ligPos.z);
	}

	Vector3 a = spotLight.GetDiretion();

	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);

	qRotY.Apply(a);

	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, a);
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickXF() * 0.01f);

	qRotX.Apply(a);

	Quaternion qRot;
	qRot.SetRotation({ 0.0f,0.0f,-1.0f }, a);

	spotLight.SetDirection(a);

	
	lightModel.UpdateWorldMatrix(spotLight.GetPosition(), qRot, g_vec3One);
}

void Game::InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}