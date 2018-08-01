#include <game/server/botplayer.h>
#include <game/server/loothandler.h>
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
	OnDeath(GameServer()->m_apPlayers[Killer]);
	MarkControllerForDestroy();
}

void CBotCharacter::MarkControllerForDestroy() {
	CBotPlayer* botController = (CBotPlayer*) m_pPlayer;
	botController->MarkForDestroy();
}

void CBotCharacter::OnDeath(CPlayer* Killer) {	
	if (Killer && Killer != m_pPlayer) {
		CLootHandler::HandleLoot(&GameServer()->m_World, m_Pos, GetBotType());
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

const char* CBotCharacter::GetDisplayName() {
	char numstr[21]; // enough to hold all numbers up to 64-bits
	return itoa(m_Health, numstr, 10);
}



