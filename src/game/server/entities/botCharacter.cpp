#include <game/server/botplayer.h>
#include <game/server/gamemodes/exp/loothandler.h>
#include "botCharacter.h"
#include "botcharacter.h"


CBotCharacter::CBotCharacter(CGameWorld *pWorld) : CCharacter(pWorld) {
	CCharacter::CCharacter(pWorld);
}

void CBotCharacter::Tick() {
	CCharacter::Tick();
	Handle();
}

void CBotCharacter::Die(int Killer, int Weapon) {
	CCharacter::Die(Killer, Weapon);
	MarkControllerForDestroy();
	OnDeath(GameServer()->m_apPlayers[Killer]);
}

void CBotCharacter::MarkControllerForDestroy() {
	CBotPlayer* botController = (CBotPlayer*) m_pPlayer;
	botController->m_MarkedForDestroy = true;
}

void CBotCharacter::OnDeath(CPlayer* Killer) {	
	if (Killer) {
		CLootHandler::HandleLoot(&GameServer()->m_World, m_Pos, m_pPlayer->m_BotType);
	}
	dbg_msg("DEBUG", "bot OnDeath.");
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

