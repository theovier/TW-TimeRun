#include <game/server/gamecontext.h>
#include <game/server/entities/doors/door.h>
#include "gatekeeperbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGatekeeperBot, MAX_CLIENTS)

CGatekeeperBot::CGatekeeperBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = 1;
	m_aWeapons[WEAPON_GUN].m_Got = true;
	m_Skin = "limekitty";
}

void CGatekeeperBot::Handle() {

}

void CGatekeeperBot::OnDeath(CPlayer* Killer) {
	GameServer()->TimeRunController()->SetDoorState(m_DoorIndex, DOOR_OPEN);
}

void CGatekeeperBot::SetDoorIndex(int DoorIndex) {
	dbg_msg("Gatekeeper", "door was set");
	m_DoorIndex = DoorIndex;
}

