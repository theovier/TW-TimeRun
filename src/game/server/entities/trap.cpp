#include "trap.h"

//ToDo: Subclass for horizontal traps or different weapons.
CTrap::CTrap(CGameWorld *pGameWorld, vec2 Pos) : CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP) {
	GameWorld()->InsertEntity(this);
	m_Pos = Pos;
	m_Active = true;
}

void CTrap::Tick() {

}


