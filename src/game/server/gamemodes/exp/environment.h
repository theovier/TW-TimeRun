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

struct CDoor : CExplorerEntity
{
	int m_Type;
	class CLaserDoor * m_Laser;
	bool m_CreateLaser;
};

#endif
