#include <game/server/entities/flag.h>
#include "flagbot.h"


MACRO_ALLOC_POOL_ID_IMPL(CFlagBot, MAX_CLIENTS)

CFlagBot::CFlagBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	m_MaxHealth = m_Health = 10;
	m_Skin = "cammostripes";

	//todo find a better way
	CGameControllerEXP * exp = (CGameControllerEXP*)GameServer()->m_pController;
	m_Flag = exp->RegisterNewCheckpoint(vec2(0, 0));
	m_Flag->Grab(this);
}

void CFlagBot::Handle() {

}

void CFlagBot::OnDeath(CPlayer* Killer) {
	m_Flag->Drop();
}
