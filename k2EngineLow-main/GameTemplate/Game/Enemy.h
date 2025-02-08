#pragma once
#include "AI/PathFinding/Path.h"

class Player;
class NaviMap;
class Enemy : public IGameObject
{
private:
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Run,
		enAnimClip_Attack,
		enAnimClip_Damage,
		enAnimClip_Death,
		enAnimClip_Num,
	};

	enum FlagStatus {
		Init,
		InGame,
		FlagStatusNum,
	};

	enum Status {
		Idel,
		Walk,
		Run,
		Chase,
		Attack,
		Damage,
		Death,
		DeathEffect,
		OutModel,
		StatusNum,
	};

	Player* player;
	NaviMap* naviMap;

	CollisionObject* m_collisionObject;

	//CCapsuleCollider* m_collidion;

	ModelInitData m_modelInitData;
	Model m_model;

	Vector3 m_pos;
	Vector3 m_moveSpeed;

	Quaternion m_rot;

	AnimationClip m_animationClipArray[enAnimClip_Num];
	Animation m_animation;
	Skeleton m_skeleton;

	nsAI::Path m_path;

	Status m_status = Status::StatusNum;
	FlagStatus m_flagStatus = FlagStatus::FlagStatusNum;


	bool isEnd;
	bool m_moveLock = false;

	int m_enemyNo;
	bool m_flag = false;

	//ここからステータス。
	int m_HP = 0;
	int m_attack = 0;
	float m_attackSpeed = 0.0f;


public:
	Enemy();
	~Enemy();
	bool Start();
	void InitModel();
	void InitCollision();
	void InitAnimation();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	void Rotation();
	void PlayAnimation();

	void SetPos(Vector3& pos)
	{
		m_pos = pos;
	}

	void SetEnemyNo(const int No)
	{
		m_enemyNo = No;
		m_flag = true;
		m_flagStatus = Init;
	}

	void DeleteEnemy()
	{
		m_status = Death;
		m_moveLock = true;
	}

	bool& GetOnFlag()
	{
		return m_flag;
	}

	Vector3& GetPos()
	{
		return m_pos;
	}

	CollisionObject* GetCol()
	{
		return m_collisionObject;
	}

	void InitData()
	{
		m_pos = { 100.0f, 0.0f, 100.0f };
		m_flagStatus = InGame;
		m_status = Idel;
		m_moveLock = false;
		m_collisionObject->SetIsEnable(true);
	}

	//初期化。
	void InitializeData()
	{
		m_pos = { 100.0f,-2000.0f,100.0f };
		m_rot = { 0.0f,0.0f,0.0f,0.0f };
		m_flag = false;
		m_collisionObject->SetPosition(m_pos);
		m_collisionObject->SetIsEnable(false);
	}
};

