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

public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	const Vector3& GetToCameraPos() const
	{
		return m_toCameraPos;
	}
};

