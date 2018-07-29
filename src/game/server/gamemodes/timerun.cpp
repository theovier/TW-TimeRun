/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <string.h> //strncmp
#include <engine/shared/config.h>

#include <game/mapitems.h>
#include <game/server/gamecontext.h>
#include <game/server/botplayer.h>
#include <game/server/player.h>
#include <game/server/entities/character.h>
#include <game/server/entities/flag.h>
#include <game/server/entities/timebonusflag.h>
#include <game/server/entities/door.h>
#include <game/server/entities/pickup.h>
#include <game/server/entities/bots/bossbot.h>
#include <game/server/entities/spawns/hammerbotspawn.h>
#include <game/server/entities/spawns/gunbotspawn.h>
#include <game/server/entities/spawns/bossbotspawn.h>
#include <game/server/entities/spawns/flagbotspawn.h>
#include <game/server/entities/spawns/botspawn.h>
#include "timerun.h"

CGameControllerTimeRun::CGameControllerTimeRun(class CGameContext *pGameServer) : IGameController(pGameServer) {
	m_pGameType = "PvE";
	m_GameFlags = GAMEFLAG_TEAMS|GAMEFLAG_FLAGS;

	// force config
	g_Config.m_SvMaxClients = 6;

	for (int i = 0; i < MAX_TURRETS; i++)
		m_Turrets[i] = 0;

	ResetDoorState();
}

void CGameControllerTimeRun::Tick() {
	IGameController::Tick();
	RemoveBotsMarkedForDestroy();
}

bool CGameControllerTimeRun::OnEntity(int Index, vec2 Pos) {
	if (IGameController::OnEntity(Index, Pos)) {
		return true;
	}

	switch (Index) {

	case ENTITY_SPAWN_BOT_HAMMER:
		m_BotSpawns[m_CurBotSpawn++] = new CHammerBotSpawn(&GameServer()->m_World, Pos, this);
		return true;

	case ENTITY_SPAWN_BOT_GUN:
		m_BotSpawns[m_CurBotSpawn++] = new CGunBotSpawn(&GameServer()->m_World, Pos, this);
		return true;

	case ENTITY_SPAWN_BOT_FLAGBEARER:
		m_BotSpawns[m_CurBotSpawn++] = new CFlagBotSpawn(&GameServer()->m_World, Pos, this);
		return true;

	case ENTITY_SPAWN_BOT_ENDBOSS:
		m_BotSpawns[m_CurBotSpawn++] = new CBossBotSpawn(&GameServer()->m_World, Pos, this);
		return true;

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

	case ENTITY_FLAGSTAND_RED:
		m_Checkpoints[m_CurFlag++] = new CCheckpoint(&GameServer()->m_World, 0, Pos, m_CurFlag + 1);
		return true;

	case ENTITY_FLAGSTAND_BLUE: {
		CTimeBonusFlag* flagBlue = new CTimeBonusFlag(&GameServer()->m_World, Pos);
		return true;
	}

	case ENTITY_DOOR_VERTICAL: {
		CDoor *pDoor = new CDoor(&GameServer()->m_World, Index);
		pDoor->m_Pos = Pos;
		return true;
	}

	default:
		return false;
	}
}

void CGameControllerTimeRun::StartRound() {
	IGameController::StartRound();
	m_ClockWasZeroTick = m_RoundStartTick;
	m_BossDefeated = false;
}

void CGameControllerTimeRun::ResetRound() {
	IGameController::ResetGame();
}

void CGameControllerTimeRun::PostReset() {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		CPlayer* player = GameServer()->m_apPlayers[i];
		if (player) {
			player->Reset();
		}
	}
}

void CGameControllerTimeRun::OnCharacterSpawn(class CCharacter *pChr) {
	pChr->IncreaseHealth(10);
	pChr->GiveWeapon(WEAPON_HAMMER, -1);
}

int CGameControllerTimeRun::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon) {
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	if (pKiller && pKiller->GetTeam() != pVictim->GetPlayer()->GetTeam()) {
		pKiller->m_Score++;
	}
	if (Weapon == WEAPON_GAME) {
		//boss was probably killed by reset. dont end the game.
		return 0;
	}
	bool isBoss = dynamic_cast<const CBossBot*>(pVictim) != nullptr;
	if (isBoss) {
		m_BossDefeated = true;
	}
	return 0;
}

void CGameControllerTimeRun::DoWincheck() {
	if (m_BossDefeated && m_GameOverTick == -1 && !m_Warmup && !GameServer()->m_World.m_ResetRequested) {
		GameServer()->SendBroadcast("The Boss has been defeated!", -1);
		EndRound();
	}
}

CCheckpoint* CGameControllerTimeRun::RegisterNewCheckpoint(vec2 Pos) {
	m_Checkpoints[m_CurFlag++] = new CCheckpoint(&GameServer()->m_World, 0, Pos, m_CurFlag + 1);
	return m_Checkpoints[m_CurFlag - 1];
}

int CGameControllerTimeRun::GetFreePlayerSlotID() {
	for (int p = g_Config.m_SvMaxClients; p < MAX_CLIENTS; p++)
		if (!GameServer()->m_apPlayers[p]) return p;
	return -1;
}

void CGameControllerTimeRun::RemoveBotsMarkedForDestroy() {
	for (int i = g_Config.m_SvMaxClients; i < MAX_CLIENTS; i++) {
		CPlayer* Player = GameServer()->m_apPlayers[i];

		if (Player && Player->IsBot()) {
			CBotPlayer* botPlayer = (CBotPlayer*)Player;
			if (botPlayer && botPlayer->IsMarkedForDestroy()) {
				RemoveBot(i);
			}
		}
	}
}

void CGameControllerTimeRun::RemoveBot(int ClientID) {
	GameServer()->OnClientDrop(ClientID, "despawn");
}

int CGameControllerTimeRun::GetDoorState(int Index) {
	return m_Door[Index].m_State;
}

void CGameControllerTimeRun::SetDoorState(int Index, int State) {
	m_Door[Index].m_State = State;
}

void CGameControllerTimeRun::ResetDoorState() {
	for (int i = 0; i < MAX_DOORS; i++) {
		m_Door[i].m_State = DOOR_CLOSED;
	}
}

void CGameControllerTimeRun::SubtractGameTime(int Seconds) {	
	//to subtract time, we have to "delay" the startTick by adding the time.
	float rewindTime = Server()->TickSpeed() * Seconds;
	float passedTime = Server()->Tick() - m_ClockWasZeroTick;

	if (rewindTime > passedTime) {
		m_RoundStartTick += passedTime;
		m_ClockWasZeroTick = Server()->Tick();
	}
	else {
		m_RoundStartTick += rewindTime;
	}
}

