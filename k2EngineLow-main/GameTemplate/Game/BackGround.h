#pragma once
class BackGround : public IGameObject
{
private:
	Model m_model;
	PhysicsStaticObject m_physicsStaticObject;

public:
	bool Start();
	void Update();
	void Init(Light& light);
	void Render(RenderContext& rc);
};

