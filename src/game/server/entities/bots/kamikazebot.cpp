#include "kamikazebot.h"


CKamikazebot::CKamikazebot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_KamikazeBotLife;
	GiveWeapon(WEAPON_KAMIKAZE, -1);
}

void CKamikazebot::SelectAppropriateWeapon(float distanceToTarget) {
	SetWeapon(WEAPON_KAMIKAZE);
}

void CKamikazebot::Tick() {
	CBotCharacter::Tick();
	m_AttackTick = Server()->Tick();
}


