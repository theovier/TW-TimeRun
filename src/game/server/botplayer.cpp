#include "botplayer.h"
#include <game/server/entities/bots/hammerbot.h>
#include <game/server/entities/bots/gunbot.h>
#include <game/server/entities/bots/flagbot.h>
#include <game/server/entities/bots/bossbot.h>


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
	m_Team = TEAM_BLUE;
	GameServer()->CreatePlayerSpawn(pSpawn->m_Pos);
	m_BotType = pSpawn->m_BotType;
	m_BotSpawn = pSpawn;

	switch (m_BotType) {
		case BOTTYPE_HAMMER:
			m_pCharacter = new(m_ClientID) CHammerbot(&GameServer()->m_World);
			break;

		case BOTTYPE_GUN:
			m_pCharacter = new(m_ClientID) CGunbot(&GameServer()->m_World);
			break;

		case BOTTYPE_FLAGBEARER: {
			m_pCharacter = new(m_ClientID) CFlagBot(&GameServer()->m_World);
			break;
		}
			
		case BOTTYPE_ENDBOSS:
			m_pCharacter = new(m_ClientID) CBossBot(&GameServer()->m_World);
			break;

		default:
			break;
		};

	str_copy(m_TeeInfos.m_SkinName, ((CBotCharacter*)m_pCharacter)->GetSkinName(), sizeof(m_TeeInfos.m_SkinName));
	m_pCharacter->Spawn(GameServer()->m_apPlayers[m_ClientID], pSpawn->m_Pos);
}

const char* CBotPlayer::GetDisplayName() {
	if (!m_pCharacter)
		return "R.I.P.";
	return ((CBotCharacter*)m_pCharacter)->GetDisplayName();
}

