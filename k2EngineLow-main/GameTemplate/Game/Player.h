#pragma once

class GameCamera;
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

	const Vector3& GetPos() const
	{
		return m_charaCon.GetPosition();
	}
};

