#include "hammerbot.h"


CHammerbot::CHammerbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = 10;
}

void CHammerbot::Handle() {
	
}

