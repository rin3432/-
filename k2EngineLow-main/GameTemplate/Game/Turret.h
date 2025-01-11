#pragma once

class GameCamera;
class Turret : public IGameObject
{
private:
	enum Status {
		Init,
		Updata,
		Num,
	};

	GameCamera* gameCamera;

	Status status = Num;

	bool m_flag;
	int m_status = 0;
	int m_executeNo = 0;

	int a = 0;	//一時的なレベルチェック用変数です、後で消して。

	std::vector<Vector3> m_turretPosList;

public:
	Turret();
	~Turret();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void OnFlag()
	{
		m_flag = true;
	}

	void SetTurretPosList(const Vector3& pos) 
	{
		m_turretPosList.push_back(pos);
	}

	struct TurretBase
	{
	private:
		Vector3 m_pos;
		Quaternion m_rot;

	public:
		void SetPos(const Vector3& pos)
		{
			m_pos = pos;
		}
		void SetPos(float x, float y, float z) 
		{
			this->SetPos({ x,y,z });
		}
		const Vector3& GetPos() const
		{
			return m_pos;
		}

		void SetRot(const Quaternion& rot)
		{
			m_rot = rot;
		}
		const Quaternion& GetRot() const
		{
			return m_rot;
		}
	};


	class AttackTurret : public TurretBase
	{
	private:

		float m_attackSpeed = 0.0f;
		float m_enemySearchRange = 0.0f;

		bool m_detect = false;			//範囲内の判定

		int m_modelNo = 0;

	public:
		ModelInitData m_initModel;
		Model m_model;

		void SetAttackSpeed(const float& speed)
		{
			m_attackSpeed = speed;
		};
		const float& GetAttackSpeed() const
		{
			return m_attackSpeed;
		};

		void SetEnemySearchRange(const float& range)
		{
			m_enemySearchRange = range;
		}
		const float& GetEnemySearchRange() const
		{
			return m_enemySearchRange;
		}

		void SetNo(int no)
		{
			m_modelNo = no;
		}
		
	};


	class SupportTurret :public TurretBase
	{
	private:
		ModelInitData m_initModel;
		Model m_model;

		float m_supportSpeed = 0.0f;
		float m_playerSearchRange = 0.0f;

		bool m_startupDetect = false;

		int m_modelNo = 0;

	public:
		void SetSupportSpeed(const float& speed)
		{
			m_supportSpeed = speed;
		}
		const float& GetSupportSpeed() const
		{
			return m_supportSpeed;
		}

		void SetPlayerSearchRange(const float& range)
		{
			m_playerSearchRange = range;
		}
		const float& GetPlayerSearchRange() const
		{
			return m_playerSearchRange;
		}

		void SetNo(int no)
		{
			m_modelNo = no;
		}
	};

	AttackTurret attackTurret;
	AttackTurret m_attackTurret[100];
};

