#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
    return true;
}

void BackGround::Updata()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_model.Draw(renderContext);
}

void BackGround::Init(Light& light)
{
	ModelInitData bgModelInitData;
	bgModelInitData.m_tkmFilePath = "Assets/modelData/MapBeta.tkm";
	//bgModelInitData.m_fxFilePath = "Assets/shader/map/SpecularMap.fx";
	bgModelInitData.m_fxFilePath = "Assets/shader/sample.fx";
	bgModelInitData.m_expandConstantBuffer = &light;
	bgModelInitData.m_expandConstantBufferSize = sizeof(light);

	m_model.Init(bgModelInitData);

	m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());
}

void BackGround::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}
