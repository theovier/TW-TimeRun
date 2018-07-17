#include "botplayer.h"
#include <game/server/entities/bots/hammerbot.h>

CBotPlayer::CBotPlayer(CGameContext *pGameServer, int ClientID, int Team) : CPlayer(pGameServer, ClientID, Team) {
	CPlayer::CPlayer(pGameServer, ClientID, Team);
}

void CBotPlayer::InitBot(struct CBotSpawn *pSpawn) {
	m_Team = 1;
	GameServer()->CreatePlayerSpawn(pSpawn->m_Pos);
	m_BotType = pSpawn->m_BotType;
	m_pBotSpawn = pSpawn;

	switch (m_BotType) {
		case BOTTYPE_HAMMER:
			m_pCharacter = new(m_ClientID) CHammerbot(&GameServer()->m_World);
			m_pCharacter->m_aWeapons[WEAPON_GUN].m_Got = false;
			break;

		case BOTTYPE_GUN:
			str_copy(m_TeeInfos.m_SkinName, "brownbear", sizeof(m_TeeInfos.m_SkinName));
			break;

		case BOTTYPE_KAMIKAZE:
			str_copy(m_TeeInfos.m_SkinName, "x_ninja", sizeof(m_TeeInfos.m_SkinName));
			m_pCharacter->m_aWeapons[WEAPON_HAMMER].m_Got = false;
			m_pCharacter->m_aWeapons[WEAPON_GUN].m_Got = false;
			m_pCharacter->GiveWeapon(WEAPON_KAMIKAZE, -1);
			m_pCharacter->m_QueuedWeapon = WEAPON_KAMIKAZE;
			m_pCharacter->m_ActiveWeapon = WEAPON_KAMIKAZE;
			break;

		case BOTTYPE_SHOTGUN:
			str_copy(m_TeeInfos.m_SkinName, "twinbop", sizeof(m_TeeInfos.m_SkinName));
			m_pCharacter->GiveWeapon(WEAPON_SHOTGUN, -1);
			break;

		case BOTTYPE_ENDBOSS:
			str_copy(m_TeeInfos.m_SkinName, "bluekitty", sizeof(m_TeeInfos.m_SkinName));
			m_pCharacter->m_aWeapons[WEAPON_GUN].m_Got = false;
			m_pCharacter->GiveWeapon(WEAPON_FREEZER, -1);
			break;

		default:
			break;
		};

	m_pCharacter->Spawn(GameServer()->m_apPlayers[m_ClientID], pSpawn->m_Pos);
}
