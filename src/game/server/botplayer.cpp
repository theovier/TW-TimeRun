#include "botplayer.h"
#include <game/server/entities/bots/hammerbot.h>
#include <game/server/entities/bots/shotgunbot.h>
#include <game/server/entities/bots/gunbot.h>
#include <game/server/entities/bots/kamikazebot.h>
#include <game/server/entities/bots/flagbot.h>
#include <game/server/entities/bots/gatekeeperbot.h>
#include <game/server/entities/bots/bossbot.h>


MACRO_ALLOC_POOL_ID_IMPL(CBotPlayer, MAX_CLIENTS)

CBotPlayer::CBotPlayer(CGameContext *pGameServer, int ClientID, int Team) : CPlayer(pGameServer, ClientID, TEAM_BLUE) {
	m_UseCustomColor = true;
}

void CBotPlayer::Tick() {
	m_Latency.m_Avg = 42;
	m_Latency.m_Max = 42;
	m_Latency.m_Min = 42;
	TickRainbow();
}

void CBotPlayer::PostTick() {
	if (m_MarkedForDestroy) {
		GameServer()->OnClientDrop(m_ClientID, "despawn");
	}
}

void CBotPlayer::Reset() {
	MarkForDestroy();
}

void CBotPlayer::Snap(int SnappingClient) {
	CPlayer::SnapIngame(SnappingClient);
}

void CBotPlayer::MarkForDestroy() {
	m_MarkedForDestroy = true;
}

void CBotPlayer::OnDisconnect(const char *pReason) {
	if (m_Spawn) {
		m_Spawn->OnSpawnlingsDeath();
	}
	KillCharacter();
}

void CBotPlayer::SpawnWithFutureRespawn(CBotSpawn *Spawn) {
	m_Spawn = Spawn;
	CBotPlayer::Spawn(Spawn->GetBotType(), Spawn->GetPos());
}

void CBotPlayer::Spawn(int BotType, vec2 SpawnPos) {
	GameServer()->CreatePlayerSpawn(SpawnPos);
	switch (BotType) {
	case BOTTYPE_HAMMER:
		m_pCharacter = new(m_ClientID) CHammerbot(&GameServer()->m_World);
		break;

	case BOTTYPE_GUN:
		m_pCharacter = new(m_ClientID) CGunbot(&GameServer()->m_World);
		break;

	case BOTTYPE_KAMIKAZE:
		m_pCharacter = new(m_ClientID) CKamikazebot(&GameServer()->m_World);
		break;

	case BOTTYPE_SHOTGUN:
		m_pCharacter = new(m_ClientID) CShotgunBot(&GameServer()->m_World);
		break;

	case BOTTYPE_FLAGBEARER:
		m_pCharacter = new(m_ClientID) CFlagBot(&GameServer()->m_World);
		break;

	case BOTTYPE_GATEKEEPER:
		m_pCharacter = new(m_ClientID) CGatekeeperBot(&GameServer()->m_World);
		break;

	case BOTTYPE_ENDBOSS:
		m_pCharacter = new(m_ClientID) CBossBot(&GameServer()->m_World);
		break;

	default:
		break;
	};

	InitTeeInfo();
	m_pCharacter->Spawn(GameServer()->m_apPlayers[m_ClientID], SpawnPos);
}

void CBotPlayer::InitTeeInfo() {
	str_copy(m_TeeInfos.m_SkinName, ((CBotCharacter*)m_pCharacter)->GetSkinName(), sizeof(m_TeeInfos.m_SkinName));
	m_TeeInfos.m_UseCustomColor = 1;
	m_TeeInfos.m_ColorBody = 10223467;
	m_TeeInfos.m_ColorFeet = 10223467;
}

const char* CBotPlayer::GetDisplayName() {
	if (!m_pCharacter)
		return "R.I.P.";
	return ((CBotCharacter*)m_pCharacter)->GetDisplayName();
}

