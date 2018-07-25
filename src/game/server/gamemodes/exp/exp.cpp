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

#include "exp.h"

CGameControllerEXP::CGameControllerEXP(class CGameContext *pGameServer)
: IGameController(pGameServer) {
	m_pGameType = "EXP";
	m_GameFlags = GAMEFLAG_TEAMS|GAMEFLAG_FLAGS;
	m_FlagEnd = NULL;
	m_Boss.m_Exist = false;

	// force config
	g_Config.m_SvMaxClients = 6;
	g_Config.m_SvScorelimit = 1;
	g_Config.m_SvTeamdamage = 0;

	for(int i = 0; i < NUM_BOTTYPES; i++)
		m_aNumBotSpawns[i] = 0;
	
	for(int i = 0; i < MAX_CHECKPOINTS; i++)
		m_aFlagsCP[i] = NULL;

	for(int i = 0; i < 3; i++)
		m_Boss.m_apShieldIcons[i] = NULL;

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
		if (m_Boss.m_Exist) {
			dbg_msg("exp", "there can't be 2 boss entities on one map");
			return false;
		}
		else {
			return OnBotEntity(BOTTYPE_ENDBOSS, Pos);
		}

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

	case ENTITY_DOOR_VERTICAL:
		m_aDoors[m_CurDoor].m_Used = true;
		m_aDoors[m_CurDoor].m_Pos = vec2(Pos.x, Pos.y - 16);
		m_aDoors[m_CurDoor].m_Type = DOOR_TYPE_VERTICAL;
		BuildDoor(m_CurDoor++);
		return true;

	case ENTITY_DOOR_HORIZONTAL:
		m_aDoors[m_CurDoor].m_Used = true;
		m_aDoors[m_CurDoor].m_Pos = vec2(Pos.x - 16, Pos.y);
		m_aDoors[m_CurDoor].m_Type = DOOR_TYPE_HORIZONTAL;
		BuildDoor(m_CurDoor++);
		return true;

	case ENTITY_FLAGSTAND_RED:
		CFlag * f = new CFlag(&GameServer()->m_World, 0, Pos);
		f->m_Pos = Pos;
		m_aFlagsCP[m_CurFlag++] = f;
		g_Config.m_SvScorelimit++;
		return true;

	case ENTITY_FLAGSTAND_BLUE:
		CFlag * f = new CFlag(&GameServer()->m_World, 1, Pos);
		f->m_Pos = Pos;
		m_FlagEnd = f;
		return true;

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
		CFlag *f = NULL;
		if(fi == m_CurFlag)
			f = m_FlagEnd;
		else
			f = m_aFlagsCP[fi];
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
				// END
				if(!m_Boss.m_Exist || GameServer()->m_apPlayers[id]->m_GameExp.m_BossKiller)
					StopClient(id);
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

bool CGameControllerEXP::CheckCommand(int ClientID, int Team, const char *aMsg)
{
	if(!strncmp(aMsg, "/info", 5) || !strncmp(aMsg, "!info", 5) || !strncmp(aMsg, "/help", 5) || !strncmp(aMsg, "/about", 6))
	{
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		GameServer()->SendChatTarget(ClientID, "EXPlorer v2.0");
		GameServer()->SendChatTarget(ClientID, "Based on the EXPlorer mod by <xush'> and <Choupom>");
		GameServer()->SendChatTarget(ClientID, "Aim: explore, fight and eventually capture the blue flag");
		GameServer()->SendChatTarget(ClientID, "Kill monsters to earn /items");
		GameServer()->SendChatTarget(ClientID, "Say /cmdlist for the command list");
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		return true;
	}
	else if(!strncmp(aMsg, "/top5", 5))
	{
		GameServer()->Top5(g_Config.m_SvMap, ClientID);
		return true;
	}
	else if(!strncmp(aMsg, "/rank", 5))
	{
		GameServer()->Rank(g_Config.m_SvMap, Server()->ClientName(ClientID), ClientID);
		return true;
	}
	else if(!strncmp(aMsg, "/cmdlist", 8) || !strncmp(aMsg, "/cmd", 4))
	{
		GameServer()->SendChatTarget(ClientID, " ");
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		GameServer()->SendChatTarget(ClientID, "COMMAND LIST");
		GameServer()->SendChatTarget(ClientID, "");
		GameServer()->SendChatTarget(ClientID, "'/info': Get info about the modification.");
		GameServer()->SendChatTarget(ClientID, "'/top5': View the top 5 players.");
		GameServer()->SendChatTarget(ClientID, "'/items': Get info about the items.");
		GameServer()->SendChatTarget(ClientID, "'/new': Restart the game.");
		GameServer()->SendChatTarget(ClientID, "'/bind': Learn how to bind a key to use an item.");
		GameServer()->SendChatTarget(ClientID, "'/game': Shows your weapons and kills.");
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		return true;
	}
	else if(!strncmp(aMsg, "/items", 6))
	{
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		GameServer()->SendChatTarget(ClientID, "ITEMS");
		GameServer()->SendChatTarget(ClientID, " ");
		GameServer()->SendChatTarget(ClientID, "Check out '/bind' to learn how to bind items.");
		GameServer()->SendChatTarget(ClientID, "Weapons: You keep it when you have it.");
		GameServer()->SendChatTarget(ClientID, "Potion: Use it to restore health.");
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		return true;
	}
	else if(!strncmp(aMsg, "/game", 5))
	{
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		GameServer()->SendChatTarget(ClientID, "GAME");
		GameServer()->SendChatTarget(ClientID, " ");
		
		char aBuf[256] = {0};
		if(GameServer()->m_apPlayers[ClientID]->m_GameExp.m_Weapons == 0)
			str_format(aBuf, sizeof(aBuf), "Weapons: none", aBuf);
		for(int i = 1; i < NUM_WEAPONS+2; i++)
		{
			if(GameServer()->m_apPlayers[ClientID]->m_GameExp.m_Weapons & (int)pow(2, i))
			{
				if(aBuf[0] == 0) str_format(aBuf, sizeof(aBuf), "Weapons: %s", GetWeaponName(i));
				else str_format(aBuf, sizeof(aBuf), "%s, %s", aBuf, GetWeaponName(i));
			}
		}
		GameServer()->SendChatTarget(ClientID, aBuf);
				
		str_format(aBuf, sizeof(aBuf), "Kills: %d", GameServer()->m_apPlayers[ClientID]->m_Score);
		GameServer()->SendChatTarget(ClientID, aBuf);
		
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		return true;
	}
	
	else if(!strncmp(aMsg, "/bind", 5))
	{
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		GameServer()->SendChatTarget(ClientID, "BIND DOCUMENTATION");
		GameServer()->SendChatTarget(ClientID, "");
		GameServer()->SendChatTarget(ClientID, "1) Open the Local Console (F1).");
		GameServer()->SendChatTarget(ClientID, "2) Type \"bind <key> say <item>\"");
		GameServer()->SendChatTarget(ClientID, "Replace <key> by the key you want to press.");
		GameServer()->SendChatTarget(ClientID, "Replace <item> by the item: potion.");
		GameServer()->SendChatTarget(ClientID, "Example: \"bind l say potion\"");
		GameServer()->SendChatTarget(ClientID, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		return true;
	}
	
	else if(!strncmp(aMsg, "/new", 4))
	{
		RestartClient(ClientID);
		return true;
	}
	
	else if(!strncmp(aMsg, "/potion", 7) || !strncmp(aMsg, "potion", 7))
	{
		Use(ClientID, "Potion");
		return true;
	}
	
	else if(!strncmp(aMsg, "/", 1))
	{
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "Unknown command: '%s'", aMsg);
		GameServer()->SendChatTarget(ClientID, aBuf);
		return true;
	}
	else
		return false;
}

void CGameControllerEXP::StartClient(int ID)
{
	GameServer()->m_apPlayers[ID]->KillCharacter(WEAPON_GAME);
	CItems Items; Items.m_Potions = 0;
	GameServer()->m_apPlayers[ID]->LoadNewGame(m_aaSpawnPoints[1][0]);
	GameServer()->m_apPlayers[ID]->m_GameExp.m_EnterTick = Server()->Tick();
	//SendItems(ID);
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

bool CGameControllerEXP::Use(int ClientID, const char *aCommand)
{
	CPlayer *p = GameServer()->m_apPlayers[ClientID];
	
	if(str_find_nocase(aCommand, "Potion"))
	{
		if(p->GetCharacter())
		{
			if(p->m_GameExp.m_Items.m_Potions > 0)
			{
				if(p->GetCharacter()->m_Health < 10)
				{
					p->m_GameExp.m_Items.m_Potions--;
					p->GetCharacter()->m_Health = 10;
					char aBuf[256];
					str_format(aBuf, sizeof(aBuf), "<Potion> used. You have %d <Potions> left.", p->m_GameExp.m_Items.m_Potions);
					GameServer()->SendChatTarget(ClientID, aBuf);
				}
				else
					GameServer()->SendChatTarget(ClientID, "You don't need to use that now!");
			}
			else
				GameServer()->SendChatTarget(ClientID, "You haven't got a <Potion>!");
		}
		return true;
	}
	
	return false;
}

