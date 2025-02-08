#include "stdafx.h"
#include "NaviMap.h"

NaviMap::NaviMap()
{

}

NaviMap::~NaviMap()
{

}

bool NaviMap::Start()
{
	m_nvmMesh.Init("Assets/nvm/naviMap2.tkn");

	return true;
}

void NaviMap::Update()
{

}
