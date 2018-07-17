#include "gunbot.h"

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_Health = 10;
}

void CGunbot::Handle() {

}

