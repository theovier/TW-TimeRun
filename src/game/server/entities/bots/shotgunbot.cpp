#include <game/server/gamecontext.h>
#include "shotgunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CShotgunBot, MAX_CLIENTS)

CShotgunBot::CShotgunBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_ShotgunBotLife;
	GiveWeapon(WEAPON_SHOTGUN, 15);
	m_Range = m_ShotgunRange;
	m_Skin = "redbopp";
}

void CShotgunBot::SelectAppropriateWeapon(float distanceToTarget) {
	if (distanceToTarget < m_ShotgunRange && m_aWeapons[WEAPON_SHOTGUN].m_Ammo > 0)
		m_QueuedWeapon = WEAPON_SHOTGUN;
	else if (distanceToTarget < m_MeeleRange)
		m_QueuedWeapon = WEAPON_HAMMER;
	else
		m_QueuedWeapon = WEAPON_GUN;
}

