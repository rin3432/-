#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const Vector3 CameraInitPos = { 0.0f,50.0f,0.0f };
}

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");
	m_toCameraPos.Set(0.0f, 20.0f, 250.0f);

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);


	return true;
}

void GameCamera::Update()
{
	m_target = m_player->GetPos();
	m_target.y += 100.0f;
	Vector3 toCameraPosOld = m_toCameraPos;

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.2f * x);
	qRot.Apply(m_toCameraPos);

	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, -1.2f * y);
	qRot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.5f) {
		m_toCameraPos = toCameraPosOld;
	}
	m_position = m_target + m_toCameraPos;
	m_target = m_target - m_toCameraPos;
	g_camera3D->SetTarget(m_position);
	g_camera3D->SetPosition(m_target);
}
