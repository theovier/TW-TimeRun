#include <game/server/gamecontext.h>
#include "door.h"
#include "doortrigger.h"

CDoorTrigger::CDoorTrigger(CGameWorld *pGameWorld, vec2 Pos, int DoorIndex) : CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP) {
	m_DoorIndex = DoorIndex;
	m_Pos = Pos;
	GameWorld()->InsertEntity(this);
}

void CDoorTrigger::Tick() {
	CCharacter *pChr = GameServer()->m_World.ClosestCharacter(m_Pos, 20.0f, 0);
	if (pChr && pChr->IsAlive()) {
		GameServer()->TimeRunController()->SetDoorState(m_DoorIndex, DOOR_OPEN);
	}
	else if (GameServer()->TimeRunController()->GetDoorState(m_DoorIndex) == DOOR_OPEN) {
		GameServer()->TimeRunController()->SetDoorState(m_DoorIndex, DOOR_CLOSED);
	}

	if (Server()->Tick() > m_AnimationTick) {
		GameServer()->CreateDeath(m_Pos + vec2(0, 20), -1);
		m_AnimationTick = Server()->Tick() + Server()->TickSpeed() * m_AnimationTime;
	}

}
