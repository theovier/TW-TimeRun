#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include <game/server/botplayer.h>
#include "turret.h"

CTurret::CTurret(CGameWorld *pGameWorld, vec2 Pos) : CTrap(pGameWorld, Pos) {
	m_Pos = Pos;
	m_DamageTaken = 0;
}

void CTurret::Tick() {
	CTrap::Tick();
	if (m_RespawnTick > 0 && Server()->Tick() > m_RespawnTick && !m_Active) {
		m_RespawnTick = -1;
		Respawn();
	}
}

void CTurret::Reset() {
	CTrap::Reset();
	m_DamageTaken = 0;
}

void CTurret::TakeDamage(int Damage, int From) {
	if (!m_Active) return;
	m_DamageTaken++;

	if (Server()->Tick() < m_DamageTakenTick + 25)	{
		GameServer()->CreateDamageInd(m_Pos, m_DamageTaken*0.25f, Damage);
	}
	else {
		m_DamageTaken = 0;
		GameServer()->CreateDamageInd(m_Pos, 0, Damage);
	}
	if (Damage) {
		m_Health -= Damage;
	}
	m_DamageTakenTick = Server()->Tick();

	GameServer()->CreateSound(GameServer()->m_apPlayers[From]->m_ViewPos, SOUND_HIT, CmaskOne(From));

	if (m_Health <= 0) {
		Die(From);
	}
	char aBuf[255];
	str_format(aBuf, sizeof(aBuf), "Turret [Health: %i]", m_Health);
	GameServer()->SendBroadcast(aBuf, From);
}

void CTurret::Die(int Killer) {
	m_Health = 0;
	m_Active = false;
	m_RespawnTick = Server()->Tick() + Server()->TickSpeed() * m_RespawnTime;

	//we don't want to cast to CPlayer if this fails
	if (Killer < 0 || Killer > MAX_CLIENTS) {
		return;
	}

	CPlayer* KillerPlayer = GameServer()->m_apPlayers[Killer];
	if (KillerPlayer) {
		KillerPlayer->m_Score++;
	}
}

void CTurret::Respawn() {
	m_Health = m_MaxHealth;
	m_DamageTaken = 0;
	m_Active = true;
}



