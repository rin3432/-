#pragma once

class GameCamera;
class Bullet;
class Turret;
class Stand;
class SpriteRender;
class Player :public IGameObject
{
private:
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Run,
		enAnimClip_Walk,
		enAnimClip_Jump,
		enAnimClip_Num,
	};

	enum Status {
		Idle,
		Run,
		Walk,
		Jump,
		Num,
	};

	Vector3 m_position;
	Vector3 m_moveSpeed;

	Quaternion m_rotation;

	CharacterController m_charaCon;

	ModelInitData unityModelInitData;
	Model m_model;

	AnimationClip m_animationClipArray[enAnimClip_Num];
	Animation m_animation;
	Skeleton m_skeleton;

	Light m_light;
	GameCamera* gameCamera;
	Bullet* bullet;
	Turret* turret;
	SpriteRender* spriteRender;

	Status m_status = Num;

	bool m_targetFlag = false;
	bool m_noAction = false;

	//ここからプレイヤーのステータス。
	int m_HP = 0;
	int m_attack = 0;
	
public:
	Player();
	~Player();
	bool Start();
	void InitAnimation();
	void Update();
	void Render(RenderContext& rc);
	void Init(Light& light);
	void Move();
	void Rotation();
	void Animation();
	const bool StandTarget();

	const Vector3& GetPos()
	{
		return m_charaCon.GetPosition();
	}
	const Quaternion& GetRot() const
	{
		return m_rotation;
	}
	//プレイヤーを操作不能にする。
	void SetOutAction()
	{
		m_noAction = true;
	}
	//プレイヤーをアクション不可の状態から復帰させる。
	void SetInAction() 
	{
		m_noAction = false;
	}
};

