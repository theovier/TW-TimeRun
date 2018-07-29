#ifndef GAME_SERVER_ENTITIES_DOOR_H
#define GAME_SERVER_ENTITIES_DOOR_H

#include <game/server/entity.h>

enum {
	DOOR_CLOSED = 0,
	DOOR_OPEN
};

class CDoor : public CEntity {

public:
	CDoor(CGameWorld *pGameWorld, vec2 Pos, int Index);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);

	int GetIndex();

private:
	int m_Index;
	int m_State;
};

#endif