#include "kamikazebot.h"


CKamikazebot::CKamikazebot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_KamikazeBotLife;
	m_aWeapons[WEAPON_HAMMER].m_Got = false;
	m_aWeapons[WEAPON_GUN].m_Got = false;
	GiveWeapon(WEAPON_KAMIKAZE, -1);
	m_Skin = "x_ninja";
}

void CKamikazebot::SelectAppropriateWeapon(float distanceToTarget) {
	m_QueuedWeapon = WEAPON_KAMIKAZE;
}


