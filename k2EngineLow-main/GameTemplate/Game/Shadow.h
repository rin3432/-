#pragma once
class Shadow : public IGameObject
{
public:
	Shadow();
	~Shadow();
	bool Start();
	bool UpdateShadow();
	void Update();

	Light m_light;
	Model m_model;

	RenderTarget shadowMap;
	Camera lightCamera;
	Model shadowModel;
	Model unityChan;
	Model bgModel;
	Sprite sprite;
};

