#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Turret.h"

namespace {
	Vector3 Zero = { 0.0f,0.0f,0.0f };
}

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	gameCamera = FindGO<GameCamera>("gameCamera");
	bullet = FindGO<Bullet>("bullet");
	turret = FindGO<Turret>("turret");
	m_charaCon.Init(25.0f, 75.0f, m_position);

	return true;
}

void Player::InitAnimation()
{
	m_skeleton.Init("Assets/modelData/player.tks");

	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle2.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/playerRun2.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/playerWalk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClipArray[enAnimClip_Jump].SetLoopFlag(false);

	m_animation.Init(
		m_skeleton,
		m_animationClipArray,
		enAnimClip_Num
	);
}

void Player::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	m_model.Draw(renderContext);

	Move();
	Rotation();
	Animation();


	//m_model.Init(unityModelInitData);
	
	m_animation.Progress(g_gameTime->GetFrameDeltaTime());

	m_model.UpdateWorldMatrix(
		m_charaCon.GetPosition(),
		m_rotation,
		g_vec3One
	);

	m_skeleton.Update(m_model.GetWorldMatrix());

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

void Player::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void Player::Init(Light& light)
{
	m_position = { 0.0f, 20.0f, 0.0f };

	InitAnimation();

	unityModelInitData.m_tkmFilePath = "Assets/modelData/player.tkm";
	unityModelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//unityModelInitData.m_fxFilePath = "Assets/shader/Bloom.fx";
	unityModelInitData.m_vsEntryPointFunc = "VSMain";
	unityModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	unityModelInitData.m_skeleton = &m_skeleton;
	unityModelInitData.m_expandConstantBuffer = &light;
	unityModelInitData.m_expandConstantBufferSize = sizeof(light);
	unityModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_model.Init(unityModelInitData);
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

	float speed = m_moveSpeed.Length();

	if (speed == 0.0f) {
		m_status = Idle;
	}
	else {
		m_status = Walk;
	}
	
		//ダッシュとジャンプ
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_moveSpeed.y = 300.0f;
	}
	if (g_pad[0]->IsPress(enButtonB))
	{
		bullet->OnFlag();
	}
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		turret->OnFlag();
	}
	if (g_pad[0]->IsPress(enButtonX))
	{
		m_moveSpeed = (right + forward) * 7.5;
		m_status = Run;
	}

	//重力。
	if (m_charaCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
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
	m_rotation.SetRotationYFromDirectionXZ(gameCamera->GetToCameraPos());
	//m_rotation.AddRotationDegX(180.0f);
}

void Player::Animation()
{
	//m_animation.Play(enAnimClip_Walk, 0.2f);
	//m_animation.Play(enAnimClip_Idle, 0.2f);
	//m_animation.Play(enAnimClip_Run, 0.2f);
	//m_animation.Play(enAnimClip_Jump, 0.2f);	//無し
	
	switch (m_status)
	{
	case Player::Idle:
		m_animation.Play(enAnimClip_Idle, 0.2f);
		break;
	case Player::Run:
		m_animation.Play(enAnimClip_Run, 0.2f);
		break;
	case Player::Walk:
		m_animation.Play(enAnimClip_Walk, 0.2f);
		break;
	case Player::Jump:
		break;
	case Player::Num:
		break;
	default:
		break;
	}
}
