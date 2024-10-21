#pragma once

class GameCamera;
class Player :public IGameObject
{
private:
	Vector3 m_position;
	Vector3 m_moveSpeed;

	Quaternion m_rotation;

	ModelInitData unityModelInitData;
	Model m_model;

	Light m_light;
	GameCamera* gameCamera;
	
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Init(Light& light);
	void Move();
	void Rotation();

	const Vector3& GetPos() const
	{
		return m_position;
	}
};

