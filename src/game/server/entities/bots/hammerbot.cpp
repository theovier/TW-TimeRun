#include "hammerbot.h"


CHammerbot::CHammerbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_HammerBotLife;
	m_aWeapons[WEAPON_GUN].m_Got = false;
}

void CHammerbot::Handle() {
	
}

