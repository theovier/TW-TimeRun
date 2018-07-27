#include "botplayer.h"
#include <game/server/entities/bots/hammerbot.h>
#include <game/server/entities/bots/gunbot.h>
#include <game/server/entities/bots/flagbot.h>
#include <game/server/entities/bots/bossbot.h>


MACRO_ALLOC_POOL_ID_IMPL(CBotPlayer, MAX_CLIENTS)

CBotPlayer::CBotPlayer(CGameContext *pGameServer, int ClientID, int Team) : CPlayer(pGameServer, ClientID, Team) {
	CPlayer::CPlayer(pGameServer, ClientID, Team);
	m_Team = TEAM_BLUE;
}

void CBotPlayer::Tick() {
	m_Latency.m_Avg = 42;
	m_Latency.m_Max = 42;
	m_Latency.m_Min = 42;
}

void CBotPlayer::Snap(int SnappingClient) {
	CPlayer::SnapIngame(SnappingClient);
}

void CBotPlayer::MarkForDestroy() {
	m_MarkedForDestroy = true;
}

void CBotPlayer::OnDisconnect(const char *pReason) {
	m_Spawn->OnSpawnlingsDeath();
	KillCharacter();
}

void CBotPlayer::InitBot(CBotSpawn *pSpawn) {
	GameServer()->CreatePlayerSpawn(pSpawn->GetPos());
	m_Spawn = pSpawn;
	switch (m_Spawn->GetBotType()) {
		case BOTTYPE_HAMMER:
			m_pCharacter = new(m_ClientID) CHammerbot(&GameServer()->m_World);
			break;

		case BOTTYPE_GUN:
			m_pCharacter = new(m_ClientID) CGunbot(&GameServer()->m_World);
			break;

		case BOTTYPE_FLAGBEARER:
			m_pCharacter = new(m_ClientID) CFlagBot(&GameServer()->m_World);
			break;
			
		case BOTTYPE_ENDBOSS:
			m_pCharacter = new(m_ClientID) CBossBot(&GameServer()->m_World);
			break;

		default:
			break;
		};

	str_copy(m_TeeInfos.m_SkinName, ((CBotCharacter*)m_pCharacter)->GetSkinName(), sizeof(m_TeeInfos.m_SkinName));
	m_pCharacter->Spawn(GameServer()->m_apPlayers[m_ClientID], pSpawn->GetPos());
}

const char* CBotPlayer::GetDisplayName() {
	if (!m_pCharacter)
		return "R.I.P.";
	return ((CBotCharacter*)m_pCharacter)->GetDisplayName();
}

