#include <engine/shared/config.h>

#include <game/collision.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

#include <game/server/entities/character.h>
#include <game/server/entities/door.h>
#include <game/server/entities/laser.h>
#include <game/server/entities/projectile.h>
#include <game/server/entities/pickup.h>

#include "exp.h"

void CGameControllerEXP::TickEnvironment() {
	TickPlayerRelatedEnvironment();
	TickPlayerUnrelatedEnvironment();
}

void CGameControllerEXP::TickPlayerRelatedEnvironment() {
	for each (CPlayer* player in GameServer()->m_apPlayers) {
		if (player) {
			TickTeleport(player);
			TickWeaponStrip(player);
			TickZones(player);
		}
	}
}

void CGameControllerEXP::TickPlayerUnrelatedEnvironment() {
	TickMines();
}

void CGameControllerEXP::TickTeleport(CPlayer* player) {
	CCharacter* character = player->GetCharacter();
	if (character) {
		vec2 charPos = character->GetPos();
		vec2 Dest = GameServer()->Collision()->Teleport(charPos.x, charPos.y);
		if (Dest.x >= 0 && Dest.y >= 0) {
			character->m_Core.m_Pos = Dest;
			character->m_Core.m_Vel = vec2(0, 0);
		}
	}
}

void CGameControllerEXP::TickWeaponStrip(CPlayer* player) {
	if (player->IsBot()) return;

	CCharacter* character = player->GetCharacter();
	if (character) {
		vec2 pos = character->GetPos();
		bool isOverlapping = GameServer()->Collision()->GetCollisionAt(pos.x, pos.y) & CCollision::COFLAG_WEAPONSTRIP;
		if (isOverlapping) {
			player->RemovePermaWeapons();
		}
	}
}

void CGameControllerEXP::TickZones(CPlayer* player) {
	CCharacter* character = player->GetCharacter();
	if (character) {
		vec2 charPos = character->GetPos();
		int collision = GameServer()->Collision()->GetCollisionAt(charPos.x, charPos.y);
		if (collision & CCollision::COLFLAG_HEALING) {
			TickHealingZone(character, player);
		}
		else if (collision & CCollision::COLFLAG_POISON) {
			TickPoisonZone(character, player);
		}
	}
}

void CGameControllerEXP::TickHealingZone(CCharacter* character, CPlayer* player) {
	bool isTicking = Server()->Tick() > player->m_GameExp.m_RegenTimer;
	if (isTicking) {
		if (character->m_Health < 10) {
			character->m_Health++;
		}
		else if (character->m_Armor < 10) {
			character->m_Armor++;
		}
		player->m_GameExp.m_RegenTimer = Server()->Tick() + (GameServer()->Tuning()->m_RegenTimer * Server()->TickSpeed());
	}
}

void CGameControllerEXP::TickPoisonZone(CCharacter* character, CPlayer* player) {
	bool isTicking = Server()->Tick() > player->m_GameExp.m_PoisonTimer;
	if (isTicking) {
		character->TakeDamage(vec2(0, 0), 1, -1, WEAPON_WORLD);
		player->m_GameExp.m_PoisonTimer = Server()->Tick() + (GameServer()->Tuning()->m_PoisonTimer * Server()->TickSpeed());
	}
}

//todo refactor
void CGameControllerEXP::TickMines() {
	for (int m = 0; m < m_CurMine; m++)
	{
		if (!m_aMines[m].m_Used)
			continue;

		if (m_aMines[m].m_Dead)
		{
			if (Server()->Tick() > m_aMines[m].m_TimerRespawn + GameServer()->Tuning()->m_RespawnTimer*Server()->TickSpeed())
				BuildMine(m);
		}
		else
		{
			//create tic-tic
			CCharacter *pClosest = GameServer()->m_World.ClosestCharacter(m_aMines[m].m_Pos, 400, NULL);
			if (pClosest)
			{
				int Mod = (int)(distance(pClosest->GetPos(), m_aMines[m].m_Pos) / 8);
				if (Mod == 0 || Server()->Tick() % Mod == 0)
					GameServer()->CreateSound(m_aMines[m].m_Pos, SOUND_HOOK_ATTACH_GROUND);
			}

			//emote close players
			CCharacter *apCloseChars[MAX_CLIENTS];
			int n = GameServer()->m_World.FindEntities(m_aMines[m].m_Pos, 300.0f, (CEntity**)apCloseChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
			for (int i = 0; i < n; i++)
			{
				if (!apCloseChars[i] || apCloseChars[i]->GetPlayer()->IsBot() || GameServer()->Collision()->IntersectLine(m_aMines[m].m_Pos, apCloseChars[i]->GetPos(), NULL, NULL, false))
					continue;
				apCloseChars[i]->m_EmoteType = EMOTE_SURPRISE;
				apCloseChars[i]->m_EmoteStop = Server()->Tick() + 1 * Server()->TickSpeed();
				if (Server()->Tick() > apCloseChars[i]->GetPlayer()->m_LastEmote + Server()->TickSpeed() * 2)
				{
					GameServer()->SendEmoticon(apCloseChars[i]->GetPlayer()->GetCID(), EMOTICON_EXCLAMATION); //!
					apCloseChars[i]->GetPlayer()->m_LastEmote = Server()->Tick();
				}
			}

			CCharacter *apCloseCharacters[MAX_CLIENTS];
			int num = GameServer()->m_World.FindEntities(m_aMines[m].m_Pos, 16.0f, (CEntity**)apCloseCharacters, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
			for (int i = 0; i < num; i++)
			{
				if (!apCloseCharacters[i] || apCloseCharacters[i]->GetPlayer()->IsBot() || GameServer()->Collision()->IntersectLine(m_aMines[m].m_Pos, apCloseCharacters[i]->GetPos(), NULL, NULL, false))
					continue;
				DestroyMine(m);
				break;
			}
		}
	}
}


void CGameControllerEXP::BuildMine(int m)
{
	m_aMines[m].m_Dead = false;
	m_aMines[m].m_TimerRespawn = 0.0f;
}

void CGameControllerEXP::DestroyMine(int m)
{
	m_aMines[m].m_Dead = true;
	m_aMines[m].m_TimerRespawn = Server()->Tick();
	
	GameServer()->CreateExplosion(m_aMines[m].m_Pos, -1, WEAPON_WORLD, false);
	GameServer()->CreateSound(m_aMines[m].m_Pos, SOUND_GRENADE_EXPLODE);
}

void CGameControllerEXP::BuildDoor(int d)
{
	m_aDoors[d].m_CreateLaser = false;
	m_aDoors[d].m_Laser = new CLaserDoor(&GameServer()->m_World, m_aDoors[d].m_Pos, m_aDoors[d].m_Type, &m_aDoors[d]);
	GameServer()->Collision()->SetDoor(m_aDoors[d].m_Laser->m_From.x, m_aDoors[d].m_Laser->m_From.y, m_aDoors[d].m_Laser->GetPos().x, m_aDoors[d].m_Laser->GetPos().y);
}
