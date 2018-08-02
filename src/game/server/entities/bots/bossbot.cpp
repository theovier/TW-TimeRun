#include "bossbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CBossBot, MAX_CLIENTS)

CBossBot::CBossBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_BossLife;
	m_EnrageDmgThreshold = GameServer()->Tuning()->m_BossEnrageDmgThreshold;
	m_Skin = "twinbop";
}

void CBossBot::Tick() {
	CBotCharacter::Tick();
	if (!m_InFight) {
		SpamZZZ();
	}
}

void CBossBot::SpamZZZ() {
	if (Server()->Tick() > m_EmoteTick + Server()->TickSpeed() * m_EmoteInterval) {
		SetEmoticon(EMOTICON_ZZZ);
	}
}

bool CBossBot::TakeDamage(vec2 Force, int Dmg, int From, int Weapon) {
	bool tookDamage = CCharacter::TakeDamage(Force, Dmg, From, Weapon);
	if (IsAlive() && m_Health < m_EnrageDmgThreshold && !m_IsEnraged) {
		Enrage();
	}
	if (!m_InFight) {
		m_InFight = true;
		SetEmoticon(EMOTICON_EXCLAMATION);
	}
	return tookDamage;
}

const char* CBossBot::GetDisplayName() {
	if (m_Health > 1) {
		std::string name = "Boss (" + std::to_string(m_Health) + ")";
		char* c = new char[name.length() + 1];
		return strcpy(c, name.c_str());
	}
	else {
		return "Boss";
	}
}

void CBossBot::Enrage() {
	//do cool stuff.
	GameServer()->SendBroadcast("Boss enrages!", -1);
	SetEmoticon(EMOTICON_EXCLAMATION);
	m_IsEnraged = true;
	if (m_pPlayer) {
		m_pPlayer->SetRainbow(true);
	}
}