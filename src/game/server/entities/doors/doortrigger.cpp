#include <game/server/gamecontext.h>
#include "door.h"
#include "doortrigger.h"

CDoorTrigger::CDoorTrigger(CGameWorld *pGameWorld, vec2 Pos, int DoorIndex) : CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP) {
	m_DoorIndex = DoorIndex;
	m_Pos = Pos;
	GameWorld()->InsertEntity(this);
}

void CDoorTrigger::Tick() {
	TickOverlap();
	TickAnimation();
}

void CDoorTrigger::TickOverlap() {
	bool CharacterNearBy = GameServer()->m_World.IsCharacterNearby(m_Pos, 20.0f);
	if (CharacterNearBy != m_LastTickCharacterNearby) {
		if (m_LastTickCharacterNearby) {
			OnOverlapStop();
		}
		else {
			OnOverlapStart();
		}
	}
	m_LastTickCharacterNearby = CharacterNearBy;
}

void CDoorTrigger::OnOverlapStart() {
	GameServer()->TimeRunController()->SetDoorState(m_DoorIndex, DOOR_OPEN);
}

void CDoorTrigger::OnOverlapStop() {
	GameServer()->TimeRunController()->SetDoorState(m_DoorIndex, DOOR_CLOSED);
}

void CDoorTrigger::TickAnimation() {
	if (Server()->Tick() > m_AnimationTick) {
		GameServer()->CreateDeath(m_Pos + vec2(0, 20), -1);
		m_AnimationTick = Server()->Tick() + Server()->TickSpeed() * m_AnimationTime;
	}
}
