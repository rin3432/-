#include "stdafx.h"
#include "Lever.h"
#include "Turret.h"
#include "GameCamera.h"
#include "Stand.h"

Lever::Lever()
{

}

Lever::~Lever()
{

}

bool Lever::Start()
{
	m_turret = FindGO<Turret>("turret");

	m_mapLevel.Init("Assets/modelData/lever/MapBate.tkl", [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"Point") == true) {
				m_turret->SetTurretPosList(objData.position);
				return true;
			}
			else if (objData.ForwardMatchName(L"dai") == true)
			{
				m_stand = NewGO<Stand>(0, "stand");
				m_stand->SetPos(objData.position);
				return true;
			}
			return true;
		});

	return true;
}

void Lever::Update()
{

}
