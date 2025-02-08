#pragma once

class Turret;
class Player;
class SpriteRender : public IGameObject 
{
private:
	enum SpriteData {
		SelectTurret,		//1
		SelectTurretFrame,	//2
		SpriteDataNum,
	};

	enum Status {
		Init,
		InUpdate,
		StatusNum,
	};

	Turret* turret;
	Player* player;

	Vector3 m_selectFramePos;

	SpriteData m_spriteData = SpriteDataNum;
	Status m_status = StatusNum;

	SpriteInitData m_spriteInit[10];
	Sprite m_sprite[10];

	bool m_display[SpriteDataNum];

	int m_selectTurret = 0;


public:
	SpriteRender();
	~SpriteRender();
	bool Start();
	void Update();
	void SelectFrameMove();
	void Render(RenderContext& rc);

	//全スプリットの初期化。
	void InitDisplay(int No) 
	{
		m_display[No] = false;
	}
	void InitSelectFramePos()
	{
		m_selectFramePos = { -370.0f,0.0f,0.0f };
	}
	//タレット選択後にのみ呼び出し。
	void OutTurretSelectSprite() 
	{
		m_display[SelectTurret] = false;
		m_display[SelectTurretFrame] = false;
	}

	void SetInDisplay(int num) 
	{
		m_display[num] = true;
		if (m_display[SelectTurret == true]) {
			m_status = Init;
		}
	}
};

