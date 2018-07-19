#include <game/server/botplayer.h>
#include <game/server/gamemodes/exp/loothandler.h>
#include "botCharacter.h"
#include "botcharacter.h"

MACRO_ALLOC_POOL_ID_IMPL(CBotCharacter, MAX_CLIENTS)

CBotCharacter::CBotCharacter(CGameWorld *pWorld) : CCharacter(pWorld) {
	CCharacter::CCharacter(pWorld);
	m_AggroRadius = 700.0f;
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
}

void CBotCharacter::Handle() {
	vec2 Target = FindTarget();
	if (!Target) return;
	Move(Target);
	SelectAppropriateWeapon(distance(Target, m_Pos));
	Aim(Target);
	Fire(Target);
}

void CBotCharacter::Move(vec2 Target) {
	// implement in subclass
}

vec2 CBotCharacter::FindTarget() {
	//default implementation
	return FindNearestTarget();
}

vec2 CBotCharacter::FindNearestTarget() {
	CCharacter* TargetChr = GameServer()->m_World.ClosestCharacter(m_Pos, m_AggroRadius, NULL);
	if (TargetChr)
		return TargetChr->m_Pos;
	else
		NULL;
}

void CBotCharacter::SelectAppropriateWeapon(float distanceToTarget) {
	// implement in subclass
}

void CBotCharacter::Aim(vec2 Target) {
	Target -= m_Pos;
	m_LatestInput.m_TargetX = Target.x;
	m_LatestInput.m_TargetY = Target.y;
	m_Input.m_TargetX = Target.x;
	m_Input.m_TargetY = Target.y;
}

void CBotCharacter::Fire(vec2 Target) {
	StopFire();
	// implement in subclass
}

void CBotCharacter::StopFire() {
	m_LatestInput.m_Fire = 0;
	m_Input.m_Fire = 0;
}



