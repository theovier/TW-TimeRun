#include "hammerbot.h"


CHammerbot::CHammerbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = 10;
	m_aWeapons[WEAPON_GUN].m_Got = false;
}

void CHammerbot::Handle() {
	
}

