#include "stdafx.h"
#include "Turret.h"
#include "GameCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Stand.h"
#include "SpriteRender.h"

#define lenght 500.0f * 500.0f

Turret::Turret()
{

}

Turret::~Turret()
{

}

bool Turret::Start()
{
	gameCamera = FindGO<GameCamera>("gameCamera");
	player = FindGO<Player>("player");
	enemy = FindGO<Enemy>("enemy");
	spriteRender = FindGO<SpriteRender>("spriteRender");

	InitAttackTurret();
	InitSupportTurret();
	

	status = Init;

	return true;
}

void Turret::Update()
{
	const std::vector<Stand*>& stands = FindGOs<Stand>("stand");

	

	switch (status)
	{
	case Init:
		if (m_flag == true)
		{
			/*for (int i = 0;i < stands.size();i++) {
				if (stands[i]->GetTargetFlag() == true) {
					attackTurret.SetPos(m_turretPosList[i]);
				}
				else {

				}
			}*/
			status = Updata;
			
			/*attackTurret.SetPos(gameCamera->GetHitPos());
			status = Updata;*/
		}
		break;

	case Updata:
		status = Init;
		m_flag = false;
		break;

	default:
		break;
	}

	for (int i = 0;i < 100;i++) {
		if (m_attackTurret[i].GetUse() == true) {
			ActionAttackTurret(i);

			m_attackTurret[i].m_model.UpdateWorldMatrix(
				m_attackTurret[i].GetPos(),
				m_attackTurret[i].GetRot(),
				m_attackTurret[i].GetScale()
			);
		}
		if (m_supportTurret[i].GetUse() == true) {
			m_supportTurret[i].m_model.UpdateWorldMatrix(
				m_supportTurret[i].GetPos(),
				m_supportTurret[i].GetRot(),
				g_vec3One
			);
		}
	}
}

void Turret::TryPut(const int listNo)
{
	// カメラのヒット座標と一番近い座標のインデックスを検索して
	Vector3 hitPos = gameCamera->GetHitPos();
	Vector3 a;
	m_listNo = listNo;

	for (int i = 0;i < m_turretPosList.size();i++){
		a = hitPos - m_turretPosList[i];
		if (i == 0){
			m_comparePos = a.Length();
			m_No = i;
		}
		else if (m_comparePos > a.Length()) {
			m_comparePos = a.Length();
			m_No = i;
		}
	}
	// 一番近いポジションが100以内の範囲にあるかどうかの条件文を書いてください
	
	Vector3 diff = player->GetPos() - m_turretPosList[m_No];
	if (diff.LengthSq() <= lenght) {
		player->SetOutAction();
		spriteRender->SetInDisplay(0);
	}
	

}
void Turret::SetTurretPos(int No)
{
	if (No == 0) {
		m_attackTurret[m_listNo].SetPos(m_turretPosList[m_No]);
		m_attackTurret[m_listNo].SetInUse();
	} else if (No == 1) {
		m_supportTurret[m_listNo].SetPos(m_turretPosList[m_No]);
		m_supportTurret[m_listNo].SetInUse();
	}
		
}

void Turret::ActionAttackTurret(int i)
{
	//仮の変数。
	Quaternion a;
	a.AddRotationDegX(-180.0f);

	if (m_attackTurret[i].SearchRangeInEnemy(enemy->GetPos()) == true) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
		EffectEmitter* laserEffectEmitter = NewGO<EffectEmitter>(0);
		laserEffectEmitter->Init(0);
		laserEffectEmitter->SetPosition(m_attackTurret[i].GetPos() + m_attackTurret[i].GetAttackStartPos());
		laserEffectEmitter->SetScale({ 30.0f,30.0f,30.0f });
		laserEffectEmitter->SetRotation(a);
		//laserEffectEmitter->SetRotation({ 0.0f,0.0f,0.0f,0.0f });
		laserEffectEmitter->Play();

	}
	}
}

void Turret::Render(RenderContext& rc)
{
	for (int i = 0;i < 100;i++) {
		if (m_attackTurret[i].GetUse() == true) {
			m_attackTurret[i].m_model.Draw(rc);
			m_attackTurret[i].m_level.Draw(rc);
		}
		if (m_supportTurret[i].GetUse() == true) {
			m_supportTurret[i].m_model.Draw(rc);
		}
	}
}

void Turret::InitAttackTurret()
{
	for (int i = 0;i < 100;i++) {
		int modelBranch = i / 10;

		switch (modelBranch)
		{
		case 0:
			m_attackTurret[i].m_initAttackModel.m_tkmFilePath = "Assets/modelData/AttackTurret.tkm";
			m_attackTurret[i].m_initAttackModel.m_fxFilePath = "Assets/shader/sample.fx";

			m_attackTurret[i].SetEnemySearchRange(10000.0f);

			EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/AttackTurretFire.efk");

			m_attackTurret[i].m_level.Init("Assets/modelData/lever/AttackTurret.tkl", [&](LevelObjectData& objData)
				{
					if (objData.ForwardMatchName(L"AttackStartPos") == true) {
						m_attackTurret[i].SetAttackStartPos(objData.position);
					}

					return true;
				});

			m_attackTurret[i].SetScale({ 1.5f,1.5f,1.5f });
			break;
			/*case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				break;*/
		default:
			m_attackTurret[i].m_initAttackModel.m_tkmFilePath = "Assets/modelData/model.tkm";
			m_attackTurret[i].m_initAttackModel.m_fxFilePath = "Assets/shader/sample.fx";
			break;
		}
		m_attackTurret[i].m_model.Init(m_attackTurret[i].m_initAttackModel);
	}
}

void Turret::InitSupportTurret()
{
	for (int i = 0;i < 100;i++) {
		m_supportTurret[i].m_initSupportModel.m_tkmFilePath = "Assets/modelData/SupportModel.tkm";
		m_supportTurret[i].m_initSupportModel.m_fxFilePath = "Assets/shader/sample.fx";

		m_supportTurret[i].m_model.Init(m_supportTurret[i].m_initSupportModel);
	}
}
