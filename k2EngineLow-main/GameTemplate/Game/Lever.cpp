#include "stdafx.h"
#include "Lever.h"
#include "Turret.h"
#include <level3D/Level.h>

Lever::Lever()
{

}

Lever::~Lever()
{

}

bool Lever::Start()
{
	m_turret = FindGO<Turret>("turret");

	m_level.Init("Assets/modelData/lever/MapBate.tkl", [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"Point") == true) {
				m_turret->SetTurretPosList(objData.position);
			}
			return true;
		});
	return true;
}

void Lever::Update()
{

}
