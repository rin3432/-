#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	gameCamera = FindGO<GameCamera>("gameCamera");
	m_charaCon.Init(25.0f, 75.0f, m_position);

	return true;
}

void Player::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	m_model.Draw(renderContext);

	Move();
	Rotation();

	m_model.Init(unityModelInitData);
	m_model.UpdateWorldMatrix(
		m_charaCon.GetPosition(),
		m_rotation,
		g_vec3One
	);

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

void Player::Render(RenderContext& rc)
{
	
}

void Player::Init(Light& light)
{
	m_position = { 0.0f, 20.0f, 0.0f };

	unityModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	unityModelInitData.m_fxFilePath = "Assets/shader/map/SpecularMap.fx";
	//unityModelInitData.m_fxFilePath = "Assets/shader/Bloom.fx";
	unityModelInitData.m_expandConstantBuffer = &light;
	unityModelInitData.m_expandConstantBufferSize = sizeof(light);
	unityModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_model.Init(unityModelInitData);
	m_model.UpdateWorldMatrix(
		m_position,
		g_quatIdentity,
		g_vec3One
	);
}

void Player::Move()
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
		
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	
	forward.y = 0.0f;
	right.y = 0.0f;
	
	right *= stickL.x * 120.0f;
	forward *= stickL.y * 120.0f;
	
	m_moveSpeed += right + forward;
	
		//ダッシュとジャンプ
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_moveSpeed.y = 300.0f;
	}
	/*if (g_pad[0]->IsPress(enButtonB))
	{
		m_moveSpeed.y = 0.0f;
	}
	if (g_pad[0]->IsPress(enButtonY))
	{
		m_moveSpeed.y = -300.0f;
	}*/
	if (g_pad[0]->IsPress(enButtonX))
	{
		m_moveSpeed = (right + forward) * 7.5;
	}

	//重力。
	if (m_charaCon.IsOnGround())
	{
		//m_moveSpeed.y = 0.0f;
	}
	else
	{
		m_moveSpeed.y -= 10.0f;
	}

	//m_position += m_moveSpeed / 10.0f;//移動速度。
	m_charaCon.Execute(m_moveSpeed, 1.0f / 20.0f);
}

void Player::Rotation()
{
	Vector3 a = gameCamera->GetToCameraPos();
	//m_rotation.SetRotationYFromDirectionXZ(a);
}
