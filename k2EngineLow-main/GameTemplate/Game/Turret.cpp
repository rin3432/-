#include "stdafx.h"
#include "Turret.h"
#include "GameCamera.h"

Turret::Turret()
{
}

Turret::~Turret()
{
}

bool Turret::Start()
{
	gameCamera = FindGO<GameCamera>("gameCamera");

	attackTurret.m_initModel.m_tkmFilePath = "Assets/modelData/model.tkm";
	attackTurret.m_initModel.m_fxFilePath = "Assets/shader/sample.fx";

	attackTurret.m_model.Init(attackTurret.m_initModel);

	for (int i = 0;i < 100;i++){
		int modelBranch = i / 10;
		
		switch (modelBranch)
		{
		case 0:
			m_attackTurret[i].m_initModel.m_tkmFilePath = "Assets/modelData/model.tkm";
			m_attackTurret[i].m_initModel.m_fxFilePath = "Assets/shader/sample.fx";
			break;
		case 1:
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
			break;
		default:
			break;
		}
	}

	status = Init;

	return true;
}

void Turret::Update()
{
	switch (status)
	{
	case Init:
		if (m_flag == true)
		{
			attackTurret.SetPos(m_turretPosList[a]);
			status = Updata;

			if (a == 1) {
				a = 0;
			} else {
				a++;
			}
			
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
	attackTurret.m_model.UpdateWorldMatrix(
		attackTurret.GetPos(),
		attackTurret.GetRot(),
		g_vec3One
	);
}

void Turret::Render(RenderContext& rc)
{
	attackTurret.m_model.Draw(rc);
}
