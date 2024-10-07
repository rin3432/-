#include "stdafx.h"
#include "Game.h"
#include "Shadow.h"


Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	shadow = NewGO<Shadow>(0,"shadow");

	InitDirLight();
	//InitPtLight();
	InitAmbLight();
	//InitSpotLight();
	
	//light.SetGroundColor(0.7f, 0.5f, 0.3f);
	//light.SetSkyColor(0.15f, 0.7f, 0.95f);
	//light.SetGroundNormal(0.3f, 0.3f, 0.3f);
	//light.SetSpecPow(5.0f);
	
	InitModel(bgModel, charaModel, lightModel, light);

	

	

	shadow->Start();

	return true;
}

void Game::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();
	
	/*Vector3 dir = directionLight.GetDiretion();

	Quaternion qRotY;
	qRotY.SetRotation(g_vec3AxisY, g_pad[0]->GetLStickXF() * 0.02);
	qRotY.Apply(dir);

	directionLight.SetDirection(dir);*/

	Quaternion qRot;
	if (g_pad[0]->IsPress(enButtonRight))
	{
		qRot.SetRotationDegY(1.0f);
	}
	else if (g_pad[0]->IsPress(enButtonLeft))
	{
		qRot.SetRotationDegY(-1.0f);
	}


	/*charaModel.UpdateWorldMatrix(
		{ -50,0,0 },
		g_quatIdentity,
		g_vec3One
	);*/

	//qRot.Apply(light.direction);
	

	// カメラも回す
	qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
	auto camPos = g_camera3D->GetPosition();
	qRot.Apply(camPos);
	g_camera3D->SetPosition(camPos);

	Vector3 rotAxis;
	auto toPos = g_camera3D->GetPosition() - g_camera3D->GetTarget();
	auto dir = toPos;
	dir.Normalize();
	rotAxis.Cross(dir, g_vec3AxisY);
	qRot.SetRotationDeg(rotAxis, g_pad[0]->GetLStickYF());
	qRot.Apply(toPos);
	g_camera3D->SetPosition(g_camera3D->GetTarget() + toPos);

	light.SetEyePos(g_camera3D->GetPosition());

	//SpotLight();
	light.SetLight(directionLight, pointLight, spotLight);

	shadow->UpdateShadow();

	RenderTarget* rtArray[] = { &offscreenRenderTarget };
	renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
	renderContext.SetRenderTargets(1, rtArray);
	renderContext.ClearRenderTargetViews(1, rtArray);

	bgModel.Draw(renderContext);
	charaModel.Draw(renderContext);
	//lightModel.Draw(renderContext);

	renderContext.WaitUntilFinishDrawingToRenderTargets(1, rtArray);

	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);

	bgModel.Draw(renderContext);
	charaModel.Draw(renderContext);
	boxModel.Draw(renderContext);
}

void Game::InitModel(Model& bgModel, Model& teapotModel, Model& lightModel, Light& light)
{
	offscreenRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	ModelInitData boxModelInitData;
	boxModelInitData.m_tkmFilePath = "Assets/modelData/box.tkm";
	boxModelInitData.m_fxFilePath = "Assets/shader/offscreenRender.fx";
	
	boxModel.Init(boxModelInitData);
	boxModel.UpdateWorldMatrix({ 100.0f,0.0f,0.0f }, g_quatIdentity, g_vec3One);


	ModelInitData bgModelInitData;
	bgModelInitData.m_tkmFilePath = "Assets/modelData/flat.tkm";
	bgModelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//bgModelInitData.m_fxFilePath = "Assets/shader/sample.fx";
	bgModelInitData.m_expandConstantBuffer = &light;
	bgModelInitData.m_expandConstantBufferSize = sizeof(light);
	bgModel.Init(bgModelInitData);

	ModelInitData unityModelInitData;
	unityModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	unityModelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//unityModelInitData.m_fxFilePath = "Assets/shader/sample.fx";
	unityModelInitData.m_expandConstantBuffer = &light;
	unityModelInitData.m_expandConstantBufferSize = sizeof(light);
	teapotModel.Init(unityModelInitData);

	teapotModel.UpdateWorldMatrix(
		{ 0.0f, 20.0f, 0.0f },
		g_quatIdentity,
		g_vec3One
	);

	boxModel.ChangeAlbedoMap(
		"",
		offscreenRenderTarget.GetRenderTargetTexture()
	);

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
	directionLight.SetDirection(0.0f, -1.0f, 0.0f);
	//directionLight.NormalizeDirection();
	directionLight.SetColor(0.5f, 0.5f, 0.5f);

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

