#include "botplayer.h"
#include <game/server/entities/bots/hammerbot.h>
#include <game/server/entities/bots/gunbot.h>


MACRO_ALLOC_POOL_ID_IMPL(CBotPlayer, MAX_CLIENTS)

CBotPlayer::CBotPlayer(CGameContext *pGameServer, int ClientID, int Team) : CPlayer(pGameServer, ClientID, Team) {
	CPlayer::CPlayer(pGameServer, ClientID, Team);
}

void CBotPlayer::Tick() {
	m_Latency.m_Avg = 42;
	m_Latency.m_Max = 42;
	m_Latency.m_Min = 42;
}

void CBotPlayer::Snap(int SnappingClient) {
	CPlayer::SnapIngame(SnappingClient);
}

void CBotPlayer::InitBot(struct CBotSpawn *pSpawn) {
	m_Team = 1;
	GameServer()->CreatePlayerSpawn(pSpawn->m_Pos);
	m_BotType = pSpawn->m_BotType;
	m_BotSpawn = pSpawn;

	switch (m_BotType) {
		case BOTTYPE_HAMMER:
			m_pCharacter = new(m_ClientID) CHammerbot(&GameServer()->m_World);
			m_pCharacter->m_aWeapons[WEAPON_GUN].m_Got = false;
			break;

		case BOTTYPE_GUN:
			m_pCharacter = new(m_ClientID) CGunbot(&GameServer()->m_World);
			str_copy(m_TeeInfos.m_SkinName, "brownbear", sizeof(m_TeeInfos.m_SkinName));
			break;

		default:
			break;
		};

	m_pCharacter->Spawn(GameServer()->m_apPlayers[m_ClientID], pSpawn->m_Pos);
}

const char * CBotPlayer::GetDisplayName() {
	if (!m_pCharacter)
		return "R.I.P.";

	int Life = m_pCharacter->m_Health;
	int MaxLife = m_pCharacter->m_MaxHealth;
	float coef = (float)Life / (float)MaxLife;

	if (coef > 0.8f)
		return "|-----|";
	else if (coef > 0.6f)
		return "|----|";
	else if (coef > 0.4f)
		return "|---|";
	else if (coef > 0.2f)
		return "|--|";
	else
		return "|-|";
}
