#include <game/server/entities/flag.h>
#include "flagbot.h"


MACRO_ALLOC_POOL_ID_IMPL(CFlagBot, MAX_CLIENTS)

CFlagBot::CFlagBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_FlagBotLife;
	m_Skin = "cammostripes";
	m_DropChance = 0;

	//todo find a better way
	CGameControllerTimeRun * exp = (CGameControllerTimeRun*)GameServer()->m_pController;
	m_Flag = exp->RegisterNewCheckpoint(vec2(0, 0));
	m_Flag->Grab(this);
}

void CFlagBot::Handle() {

}

void CFlagBot::OnDeath(CPlayer* Killer) {
	m_Flag->Drop();
}
