#pragma once
class Game : public IGameObject
{
private:
	Light light;
	DirectionalLight directionLight;
	PointLight pointLight;
	SpotLight spotLight;

	Model lightModel, bgModel, charaModel;

	
public:
	Game();
	~Game();
	bool Start();
	void Update();

	void InitModel(Model& bgModel, Model& teapotModel, Model& lightModel, Light& light);

	void InitDirLight();
	void InitPtLight();
	void InitAmbLight();
	void InitSpotLight();

	void SpotLight();
	
};

