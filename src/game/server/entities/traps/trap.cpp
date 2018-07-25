#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include <game/server/botplayer.h>
#include "trap.h"

//ToDo: Subclass for horizontal traps or different weapons.
CTrap::CTrap(CGameWorld *pGameWorld, vec2 Pos) : CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP) {
	GameWorld()->InsertEntity(this);
	m_Pos = Pos - m_Offset;
	m_Active = true;
}

void CTrap::Tick() {
	if (Server()->Tick() > m_FireTick && m_Active) {
		Arm();
	}
}

void CTrap::Arm() {
	CCharacter* closestChars[MAX_CLIENTS];
	int amountPlayersCloseBy = GameServer()->m_World.FindEntities(m_Pos, m_Range, (CEntity**)closestChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);

	for (int i = 0; i < amountPlayersCloseBy; i++) {
		CCharacter* possibleVictim = closestChars[i];
		if (Triggered(possibleVictim)) {
			Fire(possibleVictim);
			break;
		}
	}
}

bool CTrap::Triggered(CCharacter* victim) {
	CPlayer* victimController = victim->GetPlayer();
	bool IsBot = dynamic_cast<const CBotPlayer*>(victimController) != nullptr;
	if (IsBot && m_IgnoreBots) {
		return false;
	}
	return VictimInRange(victim->GetPos());
}

bool CTrap::VictimInRange(vec2 victimPos) {
	bool hasLineOfSight = HasLineOfSight(victimPos);
	bool victimInVerticalRange = VictimInVerticalRange(victimPos);
	bool victimInHorizontalRange = VictimInHorizontalRange(victimPos);
	return hasLineOfSight && victimInVerticalRange && victimInHorizontalRange;
}

bool CTrap::VictimInVerticalRange(vec2 victimPos) {
	return victimPos.y > m_Pos.y;	
}

bool CTrap::VictimInHorizontalRange(vec2 victimPos) {
	return (victimPos.x > m_Pos.x - 64) && (victimPos.x < m_Pos.x + 64);
}

bool CTrap::HasLineOfSight(vec2 victimPos) {
	return !GameServer()->Collision()->IntersectLine(m_Pos, victimPos, NULL, NULL, false);
}

void CTrap::Fire(CCharacter* at) {
	float lifeSpan = Server()->TickSpeed() * GameServer()->Tuning()->m_GrenadeLifetime;
	new CProjectile(&GameServer()->m_World, WEAPON_GRENADE, -1, m_Pos, m_Dir, lifeSpan, m_Damage, m_Explosive, 0, SOUND_GRENADE_EXPLODE, WEAPON_GRENADE);
	GameServer()->CreateSound(m_Pos, SOUND_GRENADE_FIRE);
	m_FireTick = Server()->Tick() + Server()->TickSpeed() * g_pData->m_Weapons.m_aId[WEAPON_GRENADE].m_Firedelay / 1000.0f;
}



