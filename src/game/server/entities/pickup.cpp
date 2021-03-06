/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/gamemodes/timerun.h>
#include "pickup.h"

CPickup::CPickup(CGameWorld *pGameWorld, int Type, int SubType)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
	m_Type = Type;
	m_Subtype = SubType;
	m_ProximityRadius = PickupPhysSize;

	Reset();

	GameWorld()->InsertEntity(this);
}

void CPickup::Reset()
{
	if(g_pData->m_aPickups[m_Type].m_Spawndelay > 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * g_pData->m_aPickups[m_Type].m_Spawndelay;
	else
		m_SpawnTick = -1;
}

void CPickup::Tick()
{
	// wait for respawn
	if (m_SpawnTick > 0) {
		if (Server()->Tick() > m_SpawnTick) {
			// respawn
			m_SpawnTick = -1;

			if (m_Type == POWERUP_WEAPON)
				GameServer()->CreateSound(m_Pos, SOUND_WEAPON_SPAWN);
		}
		else
			return;
	}

	TickAnims();
	TickPickup();	
}

void CPickup::TickAnims() {
	switch (m_Type) {
		case POWERUP_POTION:
			TickPotionAnim();
			break;
		default:
			break;
	}
}

void CPickup::TickPotionAnim() {
	if (Server()->Tick() > m_AnimationTimer)
	{
		int ID = -1;
		for (int i = 0; i < g_Config.m_SvMaxClients; i++)
		{
			if (GameServer()->m_apPlayers[i])
			{
				ID = i;
				break;
			}
		}
		if (ID == -1) return;
		GameServer()->CreateDeath(m_Pos, ID);
		GameServer()->CreateDeath(m_Pos, -1);
		float Sec = 0.5;
		m_AnimationTimer = Server()->Tick() + Sec*Server()->TickSpeed();
	}
}

void CPickup::TickPickup() {

	// Check if a player intersected us
	CCharacter *pChr = GameServer()->m_World.ClosestCharacter(m_Pos, 20.0f, 0);
	if (pChr && pChr->IsAlive())
	{
		CPlayer *pPlayer = pChr->GetPlayer();
		
		// player picked us up, is someone was hooking us, let them go
		int RespawnTime = -1;
		switch (m_Type) {
			case POWERUP_HEALTH: {
				if (pChr->IncreaseHealth(g_Config.m_SvPickupHealthGain)) {
					GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
					RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;
				}
				break;
			}

			case POWERUP_ARMOR: {
				if (pChr->IncreaseArmor(g_Config.m_SvPickupShieldGain)) {
					GameServer()->CreateSound(m_Pos, SOUND_PICKUP_ARMOR);
					RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;
				}
				break;
			}

			case POWERUP_WEAPON: {
				if (m_Subtype >= 0 && m_Subtype < NUM_WEAPONS + 2) {
					if (pChr->GiveWeapon(m_Subtype, 10))
					{
						RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;

						if (m_Subtype == WEAPON_GRENADE)
							GameServer()->CreateSound(m_Pos, SOUND_PICKUP_GRENADE);
						else if (m_Subtype == WEAPON_SHOTGUN)
							GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN);
						else if (m_Subtype == WEAPON_RIFLE)
							GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN);

						if (pChr->GetPlayer())
							GameServer()->SendWeaponPickup(pChr->GetPlayer()->GetCID(), m_Subtype);
					}
				}
				break;
			}

			case POWERUP_NINJA: {
				bool isNinja = pChr->m_ActiveWeapon == WEAPON_NINJA;
				if (isNinja) {
					//don't do anything
				}
				else {
					// activate ninja on target player
					pChr->GiveNinja();
					RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;

					// loop through all players, setting their emotes
					CCharacter *pC = static_cast<CCharacter *>(GameServer()->m_World.FindFirst(CGameWorld::ENTTYPE_CHARACTER));
					for (; pC; pC = (CCharacter *)pC->TypeNext())
					{
						if (pC != pChr)
							pC->SetEmote(EMOTE_SURPRISE, Server()->Tick() + Server()->TickSpeed());
					}

					pChr->SetEmote(EMOTE_ANGRY, Server()->Tick() + 1200 * Server()->TickSpeed() / 1000);
				}
				break;
			}

			case POWERUP_POTION: {
				if (pPlayer->m_GameStats.m_Items.m_Potions == 0)
					GameServer()->SendChatTarget(pPlayer->GetCID(), "Picked up: <POTION>. Use it with /potion");
				else
				{
					char aBuf[256];
					str_format(aBuf, sizeof(aBuf), "Picked up: <POTION> (%d)", pPlayer->m_GameStats.m_Items.m_Potions + 1);
					GameServer()->SendChatTarget(pPlayer->GetCID(), aBuf);
				}
				pPlayer->m_GameStats.m_Items.m_Potions++;

				GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH);
			}

			default:
				break;
		};

		HandleRespawn(RespawnTime);
	}
}

void CPickup::HandleRespawn(int RespawnTime) {
	if (RespawnTime >= 0)
		m_SpawnTick = Server()->Tick() + Server()->TickSpeed() * RespawnTime;
}


void CPickup::TickPaused()
{
	if(m_SpawnTick != -1)
		++m_SpawnTick;
}

void CPickup::Snap(int SnappingClient)
{
	if(m_SpawnTick != -1 || NetworkClipped(SnappingClient))
		return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	pP->m_X = (int)m_Pos.x;
	pP->m_Y = (int)m_Pos.y;
	pP->m_Type = RealPickup(m_Type);
	pP->m_Subtype = RealSubtype(m_Subtype);
}

int CPickup::RealPickup(int Type)
{
	if(Type == POWERUP_POTION)
		return POWERUP_HEALTH;
	return Type;
}

int CPickup::RealSubtype(int Type)
{
	if(Type == WEAPON_FREEZER)
		Type = WEAPON_RIFLE;
	return Type;
}
