#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

class NaviMap: public IGameObject
{
private:
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::PathFinding m_pathFiding;

public:
	NaviMap();
	~NaviMap();
	bool Start();
	void Update();

	void CraftNaviMap(nsAI::Path& path, Vector3 startPos, Vector3 targetPos)
	{
		m_pathFiding.Execute(
			path,
			m_nvmMesh,
			startPos,
			targetPos,
			PhysicsWorld::GetInstance(),
			50.0f,
			200.0f
		);
	}
};

