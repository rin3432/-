#pragma once
class Raytracing : public IGameObject
{
private:
	Vector3 m_position;
	Quaternion m_rotation;

public:
	Raytracing();
	~Raytracing();
	bool Start();
	void Updata();
	void Init();
};

