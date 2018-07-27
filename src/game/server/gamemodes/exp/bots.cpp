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
				if (Server()->Tick() > p->m_NobodyTimer) {
					RemoveBot(b);
				}	
			}
		}
		else
			p->m_NobodyTimer = 0;
	}
}

int CGameControllerEXP::BotCanSpawn()
{
	for(int p = g_Config.m_SvMaxClients; p < MAX_CLIENTS; p++)
		if(!GameServer()->m_apPlayers[p]) return p;
	return -1;
}


void CGameControllerEXP::RemoveBotsMarkedForDestroy() {
	for (int i = g_Config.m_SvMaxClients; i < MAX_CLIENTS; i++) {
		CPlayer* Player = GameServer()->m_apPlayers[i];

		if (Player && Player->IsBot()) {
			CBotPlayer* botPlayer = (CBotPlayer*)Player;
			if (botPlayer && botPlayer->m_MarkedForDestroy) {
				RemoveBot(i);
			}
		}
	}
}


void CGameControllerEXP::RemoveBot(int ClientID) {
	GameServer()->OnClientDrop(ClientID, "despawn");
}

