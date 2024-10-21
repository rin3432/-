#pragma once
class BackGround : public IGameObject
{
private:
	Model m_model;

public:
	bool Start();
	void Updata();
	void Init(Light& light);
	void Render(RenderContext& rc);
};

