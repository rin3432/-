#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "NaviMap.h"

namespace {
	Vector3 initScare = { 10.0f,10.0f,10.0f };
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
	DeleteGO(m_collisionObject);
}

bool Enemy::Start()
{
	player = FindGO<Player>("player");
	naviMap = FindGO<NaviMap>("naviMap");

	InitModel();
	InitAnimation();
	InitCollision();

	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/DeathEffect.efk");


	return true;
}

void Enemy::InitModel()
{
	m_pos = { 100.0f, -2000.0f, 100.0f };

	m_modelInitData.m_tkmFilePath = "Assets/modelData/skeleton.tkm";
	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";

	m_skeleton.Init("Assets/modelData/skeleton.tks");

	//unityModelInitData.m_fxFilePath = "Assets/shader/Bloom.fx";
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	m_modelInitData.m_skeleton = &m_skeleton;
	m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_model.Init(m_modelInitData);
}

void Enemy::InitCollision()
{
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateCapsule(m_pos, Quaternion::Identity, 60.0f, 200.0f);
	m_collisionObject->SetName("enemy");
	m_collisionObject->SetPosition(m_pos);
	m_collisionObject->SetIsEnable(true);

	m_collisionObject->SetIsEnableAutoDelete(false);

	//m_collidion->Init(25.0f, 100.0f);
}

void Enemy::InitAnimation()
{
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/animation/Skeleton/idel.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/animation/Skeleton/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/animation/Skeleton/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Damage].Load("Assets/animData/animation/Skeleton/damage.tka");
	m_animationClipArray[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Attack].Load("Assets/animData/animation/Skeleton/attack.tka");
	m_animationClipArray[enAnimClip_Attack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Death].Load("Assets/animData/animation/Skeleton/death.tka");
	m_animationClipArray[enAnimClip_Death].SetLoopFlag(false);
	

	m_animation.Init(
		m_skeleton,
		m_animationClipArray,
		enAnimClip_Num
	);

	m_status = Idel;
}

void Enemy::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	if (GetOnFlag() == true) {
		switch (m_flagStatus)
		{
		case Enemy::Init:
			InitData();
			break;

		case Enemy::InGame:
			m_model.Draw(renderContext);


			if (m_moveLock == false) {
				Move();
				Rotation();
			}

			PlayAnimation();

			m_animation.Progress(g_gameTime->GetFrameDeltaTime());

			m_model.UpdateWorldMatrix(
				m_pos,
				m_rot,
				initScare
			);

			if (g_pad[0]->IsTrigger(enButtonA)) {
				m_status = Attack;
			}

			m_skeleton.Update(m_model.GetWorldMatrix());
			break;

		case Enemy::FlagStatusNum:
			InitializeData();
			break;

		default:
			break;
		}
		
	}
}

void Enemy::Render(RenderContext& rc)
{
	if (GetOnFlag() == true) {
		m_model.Draw(rc);
	}
}

void Enemy::Move()
{
	bool isEnd;

	//if (g_pad[0]->IsTrigger(enButtonA)) {
		naviMap->CraftNaviMap(m_path, m_pos, player->GetPos());
	//}
		


	m_pos = m_path.Move(
		m_pos,
		10.0f,
		isEnd
	);

	if (isEnd == true)
	{
		m_status = Idel;
	}
	else
	{
		m_status = Walk;
	}

	m_collisionObject->SetPosition(m_pos);
}

void Enemy::Rotation()
{

}

void Enemy::PlayAnimation()
{
	EffectEmitter* laserEffectEmitter = NewGO<EffectEmitter>(1);

	switch (m_status)
	{
	case Enemy::Idel:
		m_animation.Play(enAnimClip_Idle, 0.2f);
		break;

	case Enemy::Walk:
		m_animation.Play(enAnimClip_Walk);
		break;

	case Enemy::Run:
		m_animation.Play(enAnimClip_Run);
		break;

	case Enemy::Chase:
		break;

	case Enemy::Attack:
		m_animation.Play(enAnimClip_Attack);
		if (m_animation.IsPlaying() == false) {
			m_status = Idel;
		}
		break;

	case Enemy::Damage:
		m_animation.Play(enAnimClip_Damage);
		break;

	case Enemy::Death:
		m_animation.Play(enAnimClip_Death);
		if (m_animation.IsPlaying() == false) {
			m_status = DeathEffect;
		}
		break;

	case Enemy::DeathEffect:
		laserEffectEmitter->Init(1);
		laserEffectEmitter->SetPosition(GetPos());
		laserEffectEmitter->SetScale({ 30.0f,30.0f,30.0f });
		laserEffectEmitter->Play();

		m_status = OutModel;
		break;

	case Enemy::OutModel:
		if (laserEffectEmitter->IsPlay() == false) {
			m_flagStatus = FlagStatusNum;
		}
		break;

	default:
		break;
	}
}
