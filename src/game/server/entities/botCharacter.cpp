#include "botcharacter.h"


CBotCharacter::CBotCharacter(CGameWorld *pWorld) : CCharacter(pWorld) {
	CCharacter::CCharacter(pWorld);
}


void CBotCharacter::Tick() {
	CCharacter::Tick();
	Handle();
}

void CBotCharacter::OnBotDeath(CPlayer* Killer, int Weapon) {
	dbg_msg("DEBUG", "bot character died.");
}

void CBotCharacter::Handle() {
	
}

void CBotCharacter::Aim(vec2 Target) {
	Target -= m_Pos;
	m_LatestInput.m_TargetX = Target.x;
	m_LatestInput.m_TargetY = Target.y;
	m_Input.m_TargetX = Target.x;
	m_Input.m_TargetY = Target.y;
}

