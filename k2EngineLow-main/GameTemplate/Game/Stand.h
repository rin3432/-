#pragma once

class GameCamera;
class Stand : public IGameObject
{
private:
	Model m_model;
	PhysicsStaticObject m_physicsStaticObject;

	Vector3 m_pos;
	Quaternion m_rot;

	GameCamera* gameCamera;

	bool m_targetStand = false;

public:
	Stand();
	~Stand();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);
	const bool Target();

	void SetPos(Vector3& pos)
	{
		m_pos = pos;
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

	const bool& GetTargetFlag() const 
	{
		return m_targetStand;
	}
};

