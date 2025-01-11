#pragma once
#include <level3D/Level.h>

class Turret;
class Lever : public IGameObject
{
private:
	Turret* m_turret;

	Level m_level;

public:
	Lever();
	~Lever();
	bool Start();
	void Update();


};

