/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <string.h> //strncmp
#include <engine/shared/config.h>

#include <game/mapitems.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>
#include <game/server/entities/character.h>
#include <game/server/entities/flag.h>
#include <game/server/entities/pickup.h>
#include <game/server/entities/bots/bossbot.h>

#include "exp.h"

CGameControllerEXP::CGameControllerEXP(class CGameContext *pGameServer)
: IGameController(pGameServer) {
	m_pGameType = "EXP";
	m_GameFlags = GAMEFLAG_TEAMS|GAMEFLAG_FLAGS;

	// force config
	g_Config.m_SvMaxClients = 6;
	g_Config.m_SvScorelimit = 1;
	g_Config.m_SvTeamdamage = 0;

	for (int i = 0; i < NUM_BOTTYPES; i++)
		m_aNumBotSpawns[i] = 0;
	for (int i = 0; i < MAX_TURRETS; i++)
		m_Turrets[i] = 0;
}

CGameControllerEXP::~CGameControllerEXP() {

}

bool CGameControllerEXP::OnEntity(int Index, vec2 Pos)
{
	if (IGameController::OnEntity(Index, Pos)) {
		return true;
	}

	switch (Index) {

		case ENTITY_SPAWN_BOT_HAMMER:
			return OnBotEntity(BOTTYPE_HAMMER, Pos);

		case ENTITY_SPAWN_BOT_GUN:
			return OnBotEntity(BOTTYPE_GUN, Pos);

		case ENTITY_SPAWN_BOT_KAMIKAZE:
			return OnBotEntity(BOTTYPE_KAMIKAZE, Pos);

		case ENTITY_SPAWN_BOT_SHOTGUN:
			return OnBotEntity(BOTTYPE_SHOTGUN, Pos);

		case ENTITY_SPAWN_BOT_FLAGBEARER:
			return OnBotEntity(BOTTYPE_FLAGBEARER, Pos);

		case ENTITY_SPAWN_BOT_ENDBOSS:
			return OnBotEntity(BOTTYPE_ENDBOSS, Pos);

		case ENTITY_TURRET_LASER:
			m_Turrets[m_CurTurret++] = new CLaserTurret(&GameServer()->m_World, Pos);
			return true;

		case ENTITY_TURRET_GUN:
			m_Turrets[m_CurTurret++] = new CGunTurret(&GameServer()->m_World, Pos);
			return true;

		case ENTITY_MINE:
			m_Mines[m_CurMine++] = new CMine(&GameServer()->m_World, Pos);
			return true;

		case ENTITY_TRAP_DOWN:
			m_Traps[m_CurTrap++] = new CTrap(&GameServer()->m_World, Pos);
			return true;

		case ENTITY_TRAP_UP:
			m_Traps[m_CurTrap++] = new CUpwardsTrap(&GameServer()->m_World, Pos);
			return true;

		case ENTITY_DOOR_VERTICAL: {
			m_aDoors[m_CurDoor].m_Used = true;
			m_aDoors[m_CurDoor].m_Pos = vec2(Pos.x, Pos.y - 16);
			m_aDoors[m_CurDoor].m_Type = DOOR_TYPE_VERTICAL;
			BuildDoor(m_CurDoor++);
			return true;
		}

		case ENTITY_DOOR_HORIZONTAL: {
			m_aDoors[m_CurDoor].m_Used = true;
			m_aDoors[m_CurDoor].m_Pos = vec2(Pos.x - 16, Pos.y);
			m_aDoors[m_CurDoor].m_Type = DOOR_TYPE_HORIZONTAL;
			BuildDoor(m_CurDoor++);
			return true;
		}
	
		case ENTITY_FLAGSTAND_RED: {
			m_Checkpoints[m_CurFlag++] = new CCheckpoint(&GameServer()->m_World, 0, Pos, m_CurFlag + 1);
			return true;
		}
		
		case ENTITY_FLAGSTAND_BLUE: {
			CFlag * flagBlue = new CFlag(&GameServer()->m_World, 1, Pos);
			return true;
		}
		
		default:
			return false;
		}
}

bool CGameControllerEXP::OnBotEntity(int BotType, vec2 pos) {
	dbg_msg("exp", "bot spawn level %d added (%d)", BotType, m_aNumBotSpawns[BotType]);
	m_aaBotSpawns[BotType][m_aNumBotSpawns[BotType]].m_Pos = pos;
	m_aaBotSpawns[BotType][m_aNumBotSpawns[BotType]].m_BotType = BotType;
	m_aaBotSpawns[BotType][m_aNumBotSpawns[BotType]].m_Spawned = false;
	m_aaBotSpawns[BotType][m_aNumBotSpawns[BotType]].m_RespawnTimer = Server()->Tick() - (GameServer()->Tuning()->m_RespawnTimer - 2)*Server()->TickSpeed();
	m_aNumBotSpawns[BotType]++;
	return true;
}

void CGameControllerEXP::Tick() {
	IGameController::Tick();
	TickBots();
	TickEnvironment();
}

void CGameControllerEXP::DoWincheck() {
	if (m_BossDefeated && m_GameOverTick == -1 && !m_Warmup && !GameServer()->m_World.m_ResetRequested) {
		GameServer()->SendBroadcast("The Boss has been defeated!", -1);
		EndRound();
	}
}

void CGameControllerEXP::PostReset() {	
	for (int i = 0; i < MAX_CLIENTS; i++) {
		CPlayer* player = GameServer()->m_apPlayers[i];
		if (player) {
			player->Respawn();
			player->m_Score = 0;
			player->m_ScoreStartTick = Server()->Tick();
			player->m_RespawnTick = Server()->Tick() + Server()->TickSpeed() / 2;
			player->RemovePermaWeapons();

			if (player->IsBot()) {
				//ToDo: DestroyBots
			}
		}
	}
}

void CGameControllerEXP::StartRound() {
	IGameController::StartRound();
	m_BossDefeated = false;
}

int CGameControllerEXP::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon) {
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);	
	
	if (pKiller && pKiller->GetTeam() != pVictim->GetPlayer()->GetTeam()) {
		pKiller->m_Score++;
	}
	bool isBoss = dynamic_cast<const CBossBot*>(pVictim) != nullptr;
	if (isBoss) {
		m_BossDefeated = true;
	}
	return 0;
}

CCheckpoint* CGameControllerEXP::RegisterNewCheckpoint(vec2 Pos) {
	m_Checkpoints[m_CurFlag++] = new CCheckpoint(&GameServer()->m_World, 0, Pos, m_CurFlag + 1);
	return m_Checkpoints[m_CurFlag - 1];
}