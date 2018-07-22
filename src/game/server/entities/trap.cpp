#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include "trap.h"

//ToDo: Subclass for horizontal traps or different weapons.
CTrap::CTrap(CGameWorld *pGameWorld, vec2 Pos) : CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP) {
	GameWorld()->InsertEntity(this);
	m_Pos = Pos - m_Offset;
	m_Active = true;
}

void CTrap::Tick() {
	
	bool isOnCooldown = Server()->Tick() < m_AttackSpeed;
	if (isOnCooldown) return;


	CCharacter* closestChars[MAX_CLIENTS];
	int amountPlayersCloseBy = GameServer()->m_World.FindEntities(m_Pos, m_Range, (CEntity**)closestChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);

	for (int i = 0; i < amountPlayersCloseBy; i++) {
		CCharacter* character = closestChars[i];
		vec2 charPos = character->GetPos();

		bool isBot = character->GetPlayer()->IsBot();
		bool hasNoLineOfSight = GameServer()->Collision()->IntersectLine(m_Pos, charPos, NULL, NULL, false);

		if (isBot || hasNoLineOfSight) {
			continue;
		}

		//todo built traps that shoot up, left, right?
		bool characterBelowTrap = charPos.y > m_Pos.y;
		bool characterInHorizontalRange = (charPos.x > m_Pos.x - 64) && (charPos.x < m_Pos.x + 64);

		if (characterBelowTrap && characterInHorizontalRange) {
			vec2 dir = vec2(0, 1);
			int lifeSpan = (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GrenadeLifetime);
			int dmg = 5;
			bool isExplosive = true;
			new CProjectile(&GameServer()->m_World, WEAPON_GRENADE, -1, m_Pos, dir, lifeSpan, dmg, isExplosive, 0, SOUND_GRENADE_EXPLODE, WEAPON_GRENADE);
			m_AttackSpeed = Server()->Tick() + g_pData->m_Weapons.m_aId[WEAPON_GRENADE].m_Firedelay * Server()->TickSpeed() / 1000.0f;
			GameServer()->CreateSound(m_Pos, SOUND_GRENADE_FIRE);
			break;
		}
	}






}


