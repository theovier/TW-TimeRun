#include <game/server/entities/superexplosion.h>
#include "kamikazebot.h"

MACRO_ALLOC_POOL_ID_IMPL(CKamikazebot, MAX_CLIENTS)

CKamikazebot::CKamikazebot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_KamikazeBotLife;
	m_TriggerRange = GameServer()->Tuning()->m_KamikazeBotTriggerRadius;
	m_ExplosionDelay = GameServer()->Tuning()->m_KamikazeBotExplosionDelay;
	m_ExplosionTick = -1;
	m_StunChance = 0;
	m_ExplosionSize = 1;
	GiveNinja();
}

void CKamikazebot::GiveNinja() {
	m_Ninja.m_ActivationTick = Server()->Tick();
	m_aWeapons[WEAPON_NINJA].m_Got = true;
	m_aWeapons[WEAPON_NINJA].m_Ammo = -1;
	if (m_ActiveWeapon != WEAPON_NINJA)
		m_LastWeapon = m_ActiveWeapon;
	m_ActiveWeapon = WEAPON_NINJA;
}

void CKamikazebot::Tick() {
	CBotCharacter::Tick();
	m_AttackTick = Server()->Tick();
	if (Server()->Tick() > m_ExplosionTick && m_ExplosionTick != -1) {
		Die(m_pPlayer->GetCID(), WEAPON_NINJA);
	}
}

void CKamikazebot::OnDeath(CPlayer* Killer) {
	CBotCharacter::OnDeath(Killer);
	Explode();
}

void CKamikazebot::SelectAppropriateWeapon(float distanceToTarget) {
	SetWeapon(WEAPON_NINJA);
	m_Ninja.m_ActivationTick = Server()->Tick();
}

void CKamikazebot::Fire(vec2 Target) {
	StopFire();
	bool HasLineOfSight = !GameServer()->Collision()->IntersectLine(m_Pos, Target, NULL, NULL);
	if (HasLineOfSight) {
		bool InRange = distance(Target, m_Pos) < m_TriggerRange;
		bool CanFire = InRange && m_ExplosionTick == -1;
		if (CanFire) {
			SetEmoticon(EMOTICON_DEVILTEE);
			m_ExplosionTick = Server()->Tick() + Server()->TickSpeed() * m_ExplosionDelay;
		}
	}
}

void CKamikazebot::Explode() {
	CSuperexplosion* boom = new CSuperexplosion(&GameServer()->m_World, m_Pos, m_pPlayer->GetCID(), WEAPON_NINJA, m_ExplosionSize);
	GameServer()->m_World.InsertEntity(boom);
	GameServer()->CreateSound(m_Pos, SOUND_GRENADE_EXPLODE);
}


