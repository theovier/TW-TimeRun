#include <game/server/botplayer.h>
#include <game/server/loothandler.h>
#include "botcharacter.h"

MACRO_ALLOC_POOL_ID_IMPL(CBotCharacter, MAX_CLIENTS)

CBotCharacter::CBotCharacter(CGameWorld *pWorld) : CCharacter(pWorld) {
	CCharacter::CCharacter(pWorld);
	m_AggroRadius = 700.0f;
	m_DespawnTime = GameServer()->Tuning()->m_BotDespawnTime;
	m_MaxArmor = 0;
}

void CBotCharacter::Tick() {
	CCharacter::Tick();
	Handle();

	if (ShouldDespawn()) {
		Despawn();
	}
}

bool CBotCharacter::ShouldDespawn() {
	return Server()->Tick() > m_DespawnTick;
}

void CBotCharacter::Despawn() {
	Die(m_pPlayer->GetCID(), WEAPON_GAME);
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
	if (!(Target.x == -1 && Target.y == -1)) {
		Move(Target);
		SelectAppropriateWeapon(distance(Target, m_Pos));
		Aim(Target);
		Hook(Target);
		Fire(Target);
		m_DespawnTick = Server()->Tick() + Server()->TickSpeed() * m_DespawnTime;
	}
}

void CBotCharacter::Move(vec2 Target) {
	bool Jumping = (bool)m_Input.m_Jump;
	m_Input.m_Direction = 0;
	m_Input.m_Jump = 0;

	if (m_Pos.x < Target.x)
		m_Input.m_Direction = 1;
	else if (m_Pos.x > Target.x)
		m_Input.m_Direction = -1;

	vec2 FuturePos;
	FuturePos.x = m_Pos.x + m_Input.m_Direction * 100;
	FuturePos.y = m_Pos.y;

	if ((GameServer()->Collision()->IntersectLine(m_Pos, FuturePos, NULL, NULL, false) || m_Pos.y > Target.y) && !Jumping)
	{
		if (IsGrounded() || m_Core.m_Vel.y > -0.3f)
			m_Input.m_Jump = 1;
	}
}

vec2 CBotCharacter::FindTarget() {
	//default implementation
	return FindNearestTarget();
}

vec2 CBotCharacter::FindNearestTarget() {
	CCharacter* TargetChr = GameServer()->m_World.ClosestCharacter(m_Pos, m_AggroRadius, this);
	if (TargetChr)
		return TargetChr->m_Pos;
	else
		return vec2(-1, -1);
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

void CBotCharacter::Hook(vec2 Target) {
	if (!GameServer()->Collision()->IntersectLine(m_Pos, Target, NULL, NULL, false) && distance(Target, m_Pos) < m_HookRange && Server()->Tick() % 10 == 0) {
		m_Input.m_Hook ^= 1;	
	}
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

void CBotCharacter::SetEmoticon(int Emotiocon) {
	if (m_pPlayer) {
		GameServer()->SendEmoticon(m_pPlayer->GetCID(), Emotiocon);
		m_pPlayer->m_LastEmote = Server()->Tick();
		m_EmoteTick = Server()->Tick() + Server()->TickSpeed() * m_EmoteInterval;
	}
}


