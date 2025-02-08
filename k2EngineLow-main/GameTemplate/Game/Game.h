#pragma once

class Player;
class GameCamera;
class BackGround;
class Shadow;
class Bullet;
class Turret;
class Lever;
class SpriteRender;
class Enemy;
class NaviMap;
class Game : public IGameObject
{
private:
	Player* player;
	Shadow* shadow;
	GameCamera* gameCamera;
	BackGround* backGround;
	Bullet* bullet;
	Turret* turret;
	Lever* level;
	Enemy* enemy;
	NaviMap* naviMap;

	Enemy* m_enemy[100];

	SpriteRender* spriteRender;

	Light light;
	DirectionalLight directionLight;
	PointLight pointLight;
	SpotLight spotLight;

	Model lightModel, bgModel, charaModel;
	Model boxModel;
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

