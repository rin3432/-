#include "stdafx.h"
#include "Stand.h"
#include "GameCamera.h"

#define lenght 100.0f * 100.0f

namespace {
	Vector3 TargetYPos = { 0.0f,40.0f,0.0f };
}

Stand::Stand()
{

}

Stand::~Stand()
{

}

bool Stand::Start()
{
	gameCamera = FindGO<GameCamera>("gameCamera");

	ModelInitData standModelInitData;
	standModelInitData.m_tkmFilePath = "Assets/modelData/dai.tkm";
	//bgModelInitData.m_fxFilePath = "Assets/shader/map/SpecularMap.fx";
	standModelInitData.m_fxFilePath = "Assets/shader/sample.fx";

	m_model.Init(standModelInitData);

	//m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());

	

	return true;
}

void Stand::Update()
{
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	if (Target() == true) {
		m_targetStand = true;
	} else {
		m_targetStand = false;
	}

	m_model.UpdateWorldMatrix(
		m_pos,
		m_rot,
		g_vec3One
	);

	m_model.Draw(renderContext);
}

void Stand::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

const bool Stand::Target()
{
	Vector3 diff = gameCamera->GetHitPos() - m_pos + TargetYPos;
	if (diff.LengthSq() <= lenght) {
		return true;
	}
	return false;
}
