#pragma once
#include <level3D/Level.h>
#

class GameCamera;
class Stand;
class Player;
class Enemy;
class SpriteRender;
class Turret : public IGameObject
{
private:
	enum Status {
		Init,
		Updata,
		Num,
	};

	GameCamera* gameCamera;
	Player* player;
	Enemy* enemy;
	SpriteRender* spriteRender;

	Status status = Num;

	bool m_flag;
	int m_status = 0;
	int m_executeNo = 0;

	float m_comparePos;	//�|�C���g���X�g�̔�r�p�|�W�V�����B
	int m_No = 0;			//��ԋ߂��|�C���g���X�g�̔ԍ��B
	int m_listNo;		//�Ώۂ̔z��ԍ��B

	int a = 0;	//�ꎞ�I�ȃ��x���`�F�b�N�p�ϐ��ł��A��ŏ����āB

	std::vector<Vector3> m_turretPosList;

public:
	Turret();
	~Turret();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void InitAttackTurret();
	void InitSupportTurret();
	void TryPut(const int listNo);
	void SetTurretPos(int No);

	void ActionAttackTurret(int No);		//�A�^�b�N�^���b�g�̍s���B

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
		Vector3 m_scale;

		bool m_inUse = false;

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

		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		void SetInUse()
		{
			m_inUse = true;
		}
		void SetOutUse()
		{
			m_inUse = false;
		}
		
		const bool GetUse() const
		{
			return m_inUse;
		}
	};


	class AttackTurret : public TurretBase
	{
	private:

		float m_attackSpeed = 0.0f;
		float m_enemySearchRange = 0.0f;

		bool m_detect = false;			//�͈͓��̔���

		int m_modelNo = 0;

		Vector3 m_attackStartPos = { 0.0f,0.0f,0.0f };

	public:
		ModelInitData m_initAttackModel;
		Model m_model;

		Level m_level;

		//�U���Ԋu�̐ݒ�B
		void SetAttackSpeed(const float& speed)
		{
			m_attackSpeed = speed;
		};
		const float& GetAttackSpeed() const
		{
			return m_attackSpeed;
		};

		//�U���͈͓��̐ݒ�B
		void SetEnemySearchRange(const float& range)
		{
			m_enemySearchRange = range;
		}
		const float& GetEnemySearchRange() const
		{
			return m_enemySearchRange;
		}

		//�U�����o����W��ݒ�B
		void SetAttackStartPos(const Vector3& pos)
		{
			m_attackStartPos += pos;
		}
		const Vector3& GetAttackStartPos() const
		{
			return m_attackStartPos;
		}

		//�͈͓��ɃG�l�~�[�����邩�̔���B
		bool SearchRangeInEnemy(Vector3& enemyPos)
		{
			Vector3 away(GetPos() - enemyPos);
			if (away.Length() <= m_enemySearchRange){
				return true;
			}
			return false;
		}
		
	};


	class SupportTurret :public TurretBase
	{
	private:
		float m_supportSpeed = 0.0f;
		float m_playerSearchRange = 0.0f;

		bool m_startupDetect = false;

		int m_modelNo = 0;

	public:
		ModelInitData m_initSupportModel;
		Model m_model;

		//���ʓK���̊Ԋu�̐ݒ�B
		void SetSupportSpeed(const float& speed)
		{
			m_supportSpeed = speed;
		}
		const float& GetSupportSpeed() const
		{
			return m_supportSpeed;
		}

		//���ʔ͈͓��̐ݒ�B
		void SetPlayerSearchRange(const float& range)
		{
			m_playerSearchRange = range;
		}
		const float& GetPlayerSearchRange() const
		{
			return m_playerSearchRange;
		}

		//�i���o�[���w��B
		void SetNo(int no)
		{
			m_modelNo = no;
		}
	};

	AttackTurret m_attackTurret[100];
	SupportTurret m_supportTurret[100];
};

