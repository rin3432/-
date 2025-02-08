#include "stdafx.h"
#include "SpriteRender.h"
#include "Player.h"
#include "Turret.h"

namespace {
	Quaternion Zero = { 0.0f,0.0f,0.0f,0.0f };
	Vector3 Scale = { 1.0f,1.0f,1.0f };
	Vector2 InitPivot = { 0.5f,0.5f };
}

SpriteRender::SpriteRender()
{

}

SpriteRender::~SpriteRender()
{

}

bool SpriteRender::Start()
{
	player = FindGO<Player>("player");
	turret = FindGO<Turret>("turret");

	m_spriteInit[SelectTurret].m_ddsFilePath[0] = "Assets/image/select.dds";
	m_spriteInit[SelectTurret].m_fxFilePath = "Assets/shader/Sprite/sprite.fx";

	m_spriteInit[SelectTurret].m_width = 1280.0f;
	m_spriteInit[SelectTurret].m_height = 300.0f;

	m_spriteInit[SelectTurret].m_alphaBlendMode = AlphaBlendMode_Trans;

	m_sprite[SelectTurret].Init(m_spriteInit[SelectTurret]);


	m_spriteInit[SelectTurretFrame].m_ddsFilePath[0] = "Assets/image/frame.dds";
	m_spriteInit[SelectTurretFrame].m_fxFilePath = "Assets/shader/Sprite/sprite.fx";

	m_spriteInit[SelectTurretFrame].m_width = 640.0f;
	m_spriteInit[SelectTurretFrame].m_height = 300.0f;

	m_spriteInit[SelectTurretFrame].m_alphaBlendMode = AlphaBlendMode_Trans;

	m_sprite[SelectTurretFrame].Init(m_spriteInit[SelectTurretFrame]);

	for (int i = 0;i < SpriteDataNum;i++) {
		InitDisplay(i);
	}

	return true;
}

void SpriteRender::Update()
{
	//タレット選択画面時処理。
	if (m_display[SelectTurret] == true) {
		switch (m_status)
		{
		case Init:
			SetInDisplay(SelectTurretFrame);
			InitSelectFramePos();
			m_status = InUpdate;
			break;

		case InUpdate:
			if (m_selectTurret >= 0) {
				if (g_pad[0]->IsTrigger(enButtonRight)) {
					m_selectFramePos.x += 720.0f;
					m_selectTurret++;
				}
			} else if (m_selectTurret <= 1) {
				if (g_pad[0]->IsTrigger(enButtonLeft)) {
					m_selectFramePos.x -= 720.0f;
					m_selectTurret--;
				}
			}
			
			
			if (g_pad[0]->IsTrigger(enButtonA) ){
				turret->SetTurretPos(m_selectTurret);
				player->SetInAction();

				OutTurretSelectSprite();
				m_status = StatusNum;
				m_selectTurret = 0;
			}
			break;

		default:
			break;
		}
		m_sprite[SelectTurretFrame].Update(m_selectFramePos, Zero, Scale, InitPivot);
	}
}

void SpriteRender::SelectFrameMove()
{

}

void SpriteRender::Render(RenderContext& rc)
{
	for (int i = 0;i < SpriteDataNum;i++) {
		if (m_display[i] == true) {
			m_sprite[i].Draw(rc);
		}
	}
	
}
