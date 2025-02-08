#pragma once
#include <level3D/Level.h>

class Turret;
class Stand;
class Lever : public IGameObject
{
private:
	Turret* m_turret;
	Stand* m_stand;

	Level m_mapLevel;
	Level m_attackTurretLevel;

	std::vector<Vector3> m_daiPosList;

public:
	Lever();
	~Lever();
	bool Start();
	void Update();


};

