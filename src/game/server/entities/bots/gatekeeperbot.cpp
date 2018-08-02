#include <game/server/gamecontext.h>
#include <game/server/entities/doors/door.h>
#include "gatekeeperbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGatekeeperBot, MAX_CLIENTS)

CGatekeeperBot::CGatekeeperBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_GatekeeperBotLife;
	m_aWeapons[WEAPON_GUN].m_Got = true;
	m_Skin = "limekitty";
}

void CGatekeeperBot::Handle() {

}

void CGatekeeperBot::OnDeath(CPlayer* Killer) {
	if (Killer != m_pPlayer) {
		//make sure we were really killed, not just despawned by our botPlayer.
		GameServer()->TimeRunController()->SetDoorState(m_DoorIndex, DOOR_OPEN);
		GameServer()->SendChat(-1, CGameContext::CHAT_ALL, "You killed a gatekeeper - a door has been opened!");
		GameServer()->CreateSoundGlobal(SOUND_HOOK_LOOP);
	}	
}

void CGatekeeperBot::SetDoorIndex(int DoorIndex) {
	m_DoorIndex = DoorIndex;
}

