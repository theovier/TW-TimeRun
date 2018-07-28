#include "character.h"
#include <game/server/gamecontext.h>
#include <game/server/gamecontroller.h>
#include <game/server/gamemodes/exp.h>
#include "timebonusflag.h"

CTimeBonusFlag::CTimeBonusFlag(CGameWorld *pGameWorld, vec2 Pos) : CEntity(pGameWorld, CGameWorld::ENTTYPE_FLAG) {
	m_Pos = Pos;
	Reset();
	GameWorld()->InsertEntity(this);
}

void CTimeBonusFlag::Tick() {
	if (m_Collected) {
		return;
	}
	CCharacter *pChr = GameServer()->m_World.ClosestCharacter(m_Pos, 20.0f, 0);
	if (pChr && pChr->IsAlive()) {
		GameServer()->EXPController()->SubtractGameTime(5);
		m_Collected = true;
	}
}

void CTimeBonusFlag::Reset() {
	m_Collected = false;
}

void CTimeBonusFlag::Snap(int SnappingClient) {
	if (m_Collected || NetworkClipped(SnappingClient))
		return;

	CNetObj_Flag *pFlag = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, m_ID, sizeof(CNetObj_Flag));
	if (!pFlag)
		return;

	pFlag->m_X = (int)m_Pos.x;
	pFlag->m_Y = (int)m_Pos.y;
	pFlag->m_Team = TEAM_BLUE;
}


