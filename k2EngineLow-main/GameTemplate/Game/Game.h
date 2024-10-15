#pragma once
class Shadow;
class Game : public IGameObject
{
private:
	Light light;
	DirectionalLight directionLight;
	PointLight pointLight;
	SpotLight spotLight;

	Model lightModel, bgModel, charaModel;
	Model boxModel;
	Shadow* shadow;
	RenderTarget offscreenRenderTarget;
	RenderTarget mainRenderTarget;
	RenderTarget luminnceRenderTarget;
	Sprite luminanceSprite;
	GaussianBlur gaussianBlur;
	Sprite finalSprite;
	Sprite copyToFrameBufferSprite;


	Vector3 plPos;

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
	void InitRootSignature(RootSignature& rs);

	void SetLight(Vector3 pos)
	{
		light.SetEyePos(pos);
	}
};

