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
	for each (CPlayer* player in GameServer()->m_apPlayers) {
		if (player) {
			TickTeleport(player);
			TickWeaponStrip(player);
			TickZones(player);
		}
	}
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

void CGameControllerEXP::BuildDoor(int d)
{
	m_aDoors[d].m_CreateLaser = false;
	m_aDoors[d].m_Laser = new CLaserDoor(&GameServer()->m_World, m_aDoors[d].m_Pos, m_aDoors[d].m_Type, &m_aDoors[d]);
	GameServer()->Collision()->SetDoor(m_aDoors[d].m_Laser->m_From.x, m_aDoors[d].m_Laser->m_From.y, m_aDoors[d].m_Laser->GetPos().x, m_aDoors[d].m_Laser->GetPos().y);
}
