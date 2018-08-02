#include "character.h"
#include <game/server/gamecontext.h>
#include <game/server/gamecontroller.h>
#include "timebonusflag.h"

CTimeBonusFlag::CTimeBonusFlag(CGameWorld *pGameWorld, vec2 Pos) : CEntity(pGameWorld, CGameWorld::ENTTYPE_FLAG) {
	m_Pos = Pos;
	m_TimeGain = GameServer()->Tuning()->m_TimeBonusFlagGain;
	Reset();
	GameWorld()->InsertEntity(this);
}

void CTimeBonusFlag::Tick() {
	if (m_Collected) {
		return;
	}
	CCharacter *pChr = GameServer()->m_World.ClosestCharacter(m_Pos, 20.0f, 0);
	if (pChr && pChr->IsAlive()) {
		GameServer()->TimeRunController()->SubtractGameTime(m_TimeGain);
		std::string name = "-" + std::to_string(m_TimeGain) + " Seconds!";
		char* c = new char[name.length() + 1];
		GameServer()->SendBroadcast(strcpy(c, name.c_str()), -1);
		GameServer()->SendChat(-1, CGameContext::CHAT_ALL, "You collected a time bonus!");
		GameServer()->CreateSoundGlobal(SOUND_CTF_RETURN);
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


