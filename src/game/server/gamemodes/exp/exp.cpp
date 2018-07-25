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

	for(int i = 0; i < NUM_BOTTYPES; i++)
		m_aNumBotSpawns[i] = 0;
	
	for(int i = 0; i < MAX_CHECKPOINTS; i++)
		m_aFlagsCP[i] = NULL;

	for (int i = 0; i < MAX_TRAPS; i++)
		m_Traps[i] = 0;

	for (int i = 0; i < MAX_TURRETS; i++)
		m_Turrets[i] = 0;

	for (int i = 0; i < MAX_MINES; i++)
		m_Mines[i] = 0;
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
		CFlag * flagRed = new CFlag(&GameServer()->m_World, 0, Pos);
		m_aFlagsCP[m_CurFlag++] = flagRed;
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

	for(int fi = 0; fi < m_CurFlag+1; fi++)
	{
		CFlag *f = m_aFlagsCP[fi];
		if(!f) // if there isn't flag end
			continue;
		
		CCharacter *apCloseCharacters[MAX_CLIENTS];
		int Num = GameServer()->m_World.FindEntities(f->GetPos(), CFlag::ms_PhysSize, (CEntity**)apCloseCharacters, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
		for(int i = 0; i < Num; i++)
		{
			if(!apCloseCharacters[i]->IsAlive() || apCloseCharacters[i]->GetPlayer()->IsBot() || apCloseCharacters[i]->GetPlayer()->GetTeam() == -1)
				continue;
			int id = apCloseCharacters[i]->GetPlayer()->GetCID();
			
			if(fi == m_CurFlag)
			{
				
			}
			else
			{
				// REGEN
				if(Server()->Tick() > apCloseCharacters[i]->GetPlayer()->m_GameExp.m_RegenTimer + GameServer()->Tuning()->m_RegenTimer*Server()->TickSpeed())
				{
					if(apCloseCharacters[i]->m_Health < 10) //regen health
					{
						apCloseCharacters[i]->m_Health++;
						apCloseCharacters[i]->GetPlayer()->m_GameExp.m_RegenTimer = Server()->Tick();
					}
					else // regen ammo
					{
						int WID = apCloseCharacters[i]->m_ActiveWeapon;
						if(apCloseCharacters[i]->m_aWeapons[WID].m_Ammo != -1)
						{
							int MaxAmmo = g_pData->m_Weapons.m_aId[WID].m_Maxammo;
							if(apCloseCharacters[i]->m_aWeapons[WID].m_Ammo < MaxAmmo)
							{
								apCloseCharacters[i]->m_aWeapons[WID].m_Ammo++;
								apCloseCharacters[i]->GetPlayer()->m_GameExp.m_RegenTimer = Server()->Tick();
							}
						}
					}
				}

				// SAVE
				if(apCloseCharacters[i]->GetPlayer()->m_GameExp.m_LastFlag != fi+1)
				{
					apCloseCharacters[i]->GetPlayer()->m_GameExp.m_LastFlag = fi+1;
					GameServer()->SendChatTarget(apCloseCharacters[i]->GetPlayer()->GetCID(), "Checkpoint reached.");
				}
			}
		}
	}
}

void CGameControllerEXP::DoWincheck() {
	if (m_BossDefeated) {
		GameServer()->SendBroadcast("The Boss has been defeated!", -1);
		//todo EndRound properly here and display winning message
	}
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

void CGameControllerEXP::StartClient(int ID)
{
	GameServer()->m_apPlayers[ID]->KillCharacter(WEAPON_GAME);
	CItems Items; Items.m_Potions = 0;
	GameServer()->m_apPlayers[ID]->LoadNewGame(m_aaSpawnPoints[1][0]);
	GameServer()->m_apPlayers[ID]->m_GameExp.m_EnterTick = Server()->Tick();
}

void CGameControllerEXP::StopClient(int ID)
{
	UpdateGame(ID);
	int min = GameServer()->m_apPlayers[ID]->m_GameExp.m_Time / 60;
	int sec = GameServer()->m_apPlayers[ID]->m_GameExp.m_Time % 60;
	char buf[512];
	str_format(buf, sizeof(buf), "'%s' finished in %d minutes %d seconds with %d kills. Good Game!", Server()->ClientName(ID), min, sec, GameServer()->m_apPlayers[ID]->m_GameExp.m_Kills);
	GameServer()->SendChatTarget(-1, buf);

	GameServer()->SaveRank(g_Config.m_SvMap, Server()->ClientName(ID), GameServer()->m_apPlayers[ID]->m_GameExp.m_Time, GameServer()->m_apPlayers[ID]->m_GameExp.m_Kills);
	
	bool GotFreezer = false;
	if(GameServer()->m_apPlayers[ID]->m_GameExp.m_Weapons & (int)pow((int)2, (int)WEAPON_FREEZER))
		GotFreezer = true;
	
	RestartClient(ID);
	
	if(GotFreezer)
		GameServer()->m_apPlayers[ID]->GiveWeaponPermanently(WEAPON_FREEZER, -1);
}

void CGameControllerEXP::RestartClient(int ID)
{
	StartClient(ID);
	GameServer()->SendChatTarget(ID, "Game restarted.");
}

void CGameControllerEXP::UpdateGame(int ID)
{
	int DiffTick = Server()->Tick() - GameServer()->m_apPlayers[ID]->m_GameExp.m_EnterTick;
	GameServer()->m_apPlayers[ID]->m_GameExp.m_Time += (int) (DiffTick / Server()->TickSpeed());
	GameServer()->m_apPlayers[ID]->m_GameExp.m_EnterTick = Server()->Tick();
	GameServer()->m_apPlayers[ID]->m_GameExp.m_Kills = GameServer()->m_apPlayers[ID]->m_Score;
}

const char *CGameControllerEXP::GetWeaponName(int WID)
{
	if(WID == WEAPON_HAMMER)
		return "Hammer";
	else if(WID == WEAPON_GUN)
		return "Gun";
	else if(WID == WEAPON_SHOTGUN)
		return "Shotgun";
	else if(WID == WEAPON_GRENADE)
		return "Grenade";
	else if(WID == WEAPON_RIFLE)
		return "Rifle";
	else if(WID == WEAPON_NINJA)
		return "Ninja";
	else if(WID == WEAPON_KAMIKAZE)
		return "Kamikaze";
	else if(WID == WEAPON_FREEZER)
		return "Freezer";
	return "?";
}

void CGameControllerEXP::Use(int ClientID, const char *aCommand) {
	CPlayer *p = GameServer()->m_apPlayers[ClientID];
	if(str_find_nocase(aCommand, "Potion")) {
		p->UseItem(POTION);
	}
}

void CGameControllerEXP::Snap(int SnappingClient) {
	IGameController::Snap(SnappingClient);
}