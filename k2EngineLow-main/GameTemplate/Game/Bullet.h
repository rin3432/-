#pragma once
class GameCamera;
class Player;
class Enemy;
class Bullet : public IGameObject
{
private:
	GameCamera* gameCamera;
	Player* player;
	Enemy* enemy;

	ModelInitData m_modelInitData;
	Model m_model;

	Quaternion m_rotation;
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_initPos;

	CollisionObject* m_collision;

	bool flag = false;
	bool initAfter = false;
	float deleteTime = 0.0f;
	
public:
	Bullet();
	~Bullet();
	bool Start();
	void Update();

	void InitModel();
	void InitPos();
	void InitRot();
	void InitCollision();

	void Move();
	void Rotation();
	void DeleteTime();

	void HitJudgment();

	void Render(RenderContext& rc);

	void Init()
	{
		m_position = { 0.0f,-1000.0f,0.0f };
		m_rotation = { 0.0f,0.0f,0.0f,0.0f };
		flag = false;
		initAfter = false;
		deleteTime = 3.0f;
	}

	void OnFlag()
	{
		flag = true;
		deleteTime = 3.0f;
	}

	bool& GetFlag()
	{
		return flag;
	}
};

