#include <game/server/gamecontext.h>
#include "door.h"

CDoor::CDoor(CGameWorld *pGameWorld, vec2 Pos, int Index) : CEntity(pGameWorld, CGameWorld::ENTTYPE_DOOR) {
	m_Index = Index;
	m_Pos = Pos;
	m_State = DOOR_CLOSED;
	GameWorld()->InsertEntity(this);
}

void CDoor::Reset() {
	if (m_Index == -1)
		GameServer()->m_World.DestroyEntity(this);
	GameServer()->TimeRunController()->SetDoorState(m_Index, DOOR_CLOSED);
}

void CDoor::Tick() {
	if (m_Index > -1)
		m_State = GameServer()->TimeRunController()->GetDoorState(m_Index);
	if (m_State == DOOR_OPEN)
		return;

	CCharacter *apCloseCCharacters[MAX_CLIENTS];
	int Num = GameServer()->m_World.FindEntities(m_Pos, 8.0f, (CEntity**)apCloseCCharacters, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	for (int i = 0; i < Num; i++){
		if (!apCloseCCharacters[i]->IsAlive() || apCloseCCharacters[i]->GetPlayer()->GetTeam() == TEAM_SPECTATORS || GameServer()->Collision()->IntersectLine(m_Pos, apCloseCCharacters[i]->m_Pos, NULL, NULL))
			continue;
		apCloseCCharacters[i]->m_HittingDoor = true;
		apCloseCCharacters[i]->m_PushDirection = normalize(apCloseCCharacters[i]->m_OldPos - m_Pos);
		apCloseCCharacters[i]->m_Ninja.m_CurrentMoveTime = 0;
	}
}

void CDoor::Snap(int SnappingClient) {
	if (m_State == DOOR_OPEN || NetworkClipped(SnappingClient))
		return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if (!pP)
		return;

	pP->m_X = (int)m_Pos.x;
	pP->m_Y = (int)m_Pos.y;
	pP->m_Type = POWERUP_ARMOR;
	pP->m_Subtype = 0;
}

int CDoor::GetIndex() {
	return m_Index;
}
