#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

enum CDoorTypes
{
	DOOR_TYPE_VERTICAL=0,
	DOOR_TYPE_HORIZONTAL
};

struct CExplorerEntity
{
	bool m_Used;
	vec2 m_Pos;
	CExplorerEntity() { m_Used = false; }
};

struct CMine : CExplorerEntity
{
	bool m_Dead;
	float m_TimerRespawn;
};

struct CDoor : CExplorerEntity
{
	int m_Type;
	class CLaserDoor * m_Laser;
	bool m_CreateLaser;
};

enum
{
	MAX_TURRETS=256,
	MAX_MINES=256,
	MAX_TRAPS=256,
	MAX_DOORS=128
};

#endif
