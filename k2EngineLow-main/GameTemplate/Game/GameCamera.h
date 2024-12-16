#pragma once

class Player;
class GameCamera : public IGameObject
{
private:
	Player* m_player;

	Vector3 m_toCameraPos;	//íçéãì_ÅB
	Vector3 m_toPosDir;
	Vector3 m_position;
	Vector3 m_target;

	Vector3 m_endForwardPos;
	Vector3 m_hitPos;

	Model m_model;
	Quaternion m_rot;

public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	const Vector3& GetToCameraPos() const
	{
		return m_toCameraPos;
	}

	Vector3& GetHitPos()
	{
		return m_hitPos;
	}
};

