#include <engine/shared/config.h>

#include <game/server/gamecontext.h>
#include <game/server/player.h>
#include <game/server/entities/character.h>
#include <game/server/entities/pickup.h>
#include "exp.h"
#include "../../botplayer.h"
#include "bots.h"

void CGameControllerEXP::TickBots()
{
	RemoveBotsMarkedForDestroy();

	// CHECK FOR NOBODY
	for(int b = g_Config.m_SvMaxClients; b < MAX_CLIENTS; b++)
	{
		CPlayer *p = GameServer()->m_apPlayers[b];
		if(!p || !p->GetCharacter())
			continue;
		
		bool Nobody = true;
		for(int i = 0; i < g_Config.m_SvMaxClients; i++)
		{
			if(GameServer()->m_apPlayers[i] && !p->GetCharacter()->NetworkClipped(i))
			{
				Nobody = false;
				break;
			}
		}
		
		if(Nobody)
		{
			if(p->m_NobodyTimer == 0)
			{
				p->m_NobodyTimer = Server()->Tick() + 10.0f*Server()->TickSpeed();
			}
			else
			{
				if(Server()->Tick() > p->m_NobodyTimer)
					RemoveBot(b, false);
			}
		}
		else
			p->m_NobodyTimer = 0;
	}
	
	
	// CHECK FOR BOT SPAWNS
	for(int l = 0; l < NUM_BOTTYPES; l++)
	{
		for(int s = 0; s < m_aNumBotSpawns[l]; s++)
		{
			if(BotCanSpawn() == -1)
				break;
			
			CBotSpawn *pSpawn = &m_aaBotSpawns[l][s];
			if(pSpawn->m_Spawned)
				continue;
			if(Server()->Tick() < pSpawn->m_RespawnTimer + GameServer()->Tuning()->m_RespawnTimer*Server()->TickSpeed())
				continue;
			
			for(int p = 0; p < g_Config.m_SvMaxClients; p++)
			{
				if(!GameServer()->m_apPlayers[p] || !GameServer()->m_apPlayers[p]->GetCharacter())
					continue;
				
				if(distance(GameServer()->m_apPlayers[p]->GetCharacter()->GetPos(), pSpawn->m_Pos) < 700.0f)
				{
					BotSpawn(pSpawn);
					break;
				}
			}
		}
	}
}

void CGameControllerEXP::RemoveBotsMarkedForDestroy() {
	for (int i = g_Config.m_SvMaxClients; i < MAX_CLIENTS; i++) {
		CPlayer* Player = GameServer()->m_apPlayers[i];
		
		//too expensive to dyncast all the time?
		bool IsBot = dynamic_cast<const CBotPlayer*>(Player) != nullptr;
		if (IsBot) {
			CBotPlayer* botPlayer = (CBotPlayer*)Player;
			if (botPlayer && botPlayer->m_MarkedForDestroy) {
				RemoveBot(i, true);
			}
		}
	}
}

int CGameControllerEXP::BotCanSpawn()
{
	for(int p = g_Config.m_SvMaxClients; p < MAX_CLIENTS; p++)
		if(!GameServer()->m_apPlayers[p]) return p;
	return -1;
}

void CGameControllerEXP::BotSpawn(CBotSpawn *pSpawn)
{
	CEntity *pEnts[1] = {0};
	int NumEnts = GameServer()->m_World.FindEntities(pSpawn->m_Pos, 28.0f, pEnts, 1, CGameWorld::ENTTYPE_CHARACTER);
	if(NumEnts != 0)
		return;

	int BID = BotCanSpawn();
	if(BID == -1) return;
	
	GameServer()->OnBotClientConnected(BID);
	CBotPlayer* botPlayer = (CBotPlayer*) GameServer()->m_apPlayers[BID];
	botPlayer->InitBot(pSpawn);


	//GameServer()->m_apPlayers[BID]->SetTeam(0);
	pSpawn->m_Spawned = true;
}

void CGameControllerEXP::RemoveBot(int ID, bool Killed) {
	CBotPlayer* Bot = (CBotPlayer*)GameServer()->m_apPlayers[ID];
	CBotSpawn* Spawn = Bot->m_BotSpawn;
	Spawn->m_Spawned = false;

	if(Killed)
		Spawn->m_RespawnTimer = Server()->Tick();
		
	delete Bot->m_pCharacter;
	Bot->m_pCharacter = NULL;
	delete GameServer()->m_apPlayers[ID];
	GameServer()->m_apPlayers[ID] = NULL;
}
