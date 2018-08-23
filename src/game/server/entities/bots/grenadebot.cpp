#include "grenadebot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGrenadeBot, MAX_CLIENTS)

CGrenadeBot::CGrenadeBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_GrenadeBotLife;
	GiveWeapon(WEAPON_GRENADE, m_MaxGrenadeAmmo);
	SetWeapon(WEAPON_GRENADE);
	m_AttackSpeed = 1.6f;
	m_Range = m_GrenadeRange;
	m_Skin = "warpaint";
	m_DropChance = 0.75f;
}

void CGrenadeBot::Aim(vec2 Target) {
	//todo: modify aiming for grenades
	CBotCharacter::Aim(Target);
}

void CGrenadeBot::StopReload() {
	CBotCharacter::StopReload();
	m_aWeapons[m_ActiveWeapon].m_Ammo = m_MaxGrenadeAmmo;
}


