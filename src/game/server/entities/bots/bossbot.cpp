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
	if (m_IsEnraged) {
		TickEnrage();
	}
}

void CBossBot::TickEnrage() {
	SetEmote(EMOTE_ANGRY, Server()->Tick() + Server()->TickSpeed());
}

void CBossBot::SpamZZZ() {
	if (Server()->Tick() > m_EmoteTick + Server()->TickSpeed() * m_EmoteInterval) {
		SetEmote(EMOTE_BLINK, Server()->Tick() + Server()->TickSpeed() * 2 * m_EmoteInterval);
		SetEmoticon(EMOTICON_ZZZ);
	}
}

bool CBossBot::TakeDamage(vec2 Force, int Dmg, int From, int Weapon) {
	bool tookDamage = CCharacter::TakeDamage(Force, Dmg, From, Weapon);
	if (IsAlive() && m_Health < m_EnrageDmgThreshold && !m_IsEnraged) {
		StartEnrage();
	}
	if (!m_InFight) {
		m_InFight = true;
		SetEmoticon(EMOTICON_ZOMG);
		SetEmote(EMOTE_ANGRY, Server()->Tick() + Server()->TickSpeed());
	}
	return tookDamage;
}

void CBossBot::StartEnrage() {
	//do cool stuff.
	GameServer()->SendBroadcast("Boss enrages!", -1);
	SetEmoticon(EMOTICON_EXCLAMATION);
	
	m_IsEnraged = true;
	if (m_pPlayer) {
		m_pPlayer->SetRainbow(true);
	}
	//SummonMinions();
	FreezeAllPlayers();
}

void CBossBot::SummonMinions() {
	//GameServer()->SendBroadcast("Boss calls for help!", -1);
	
	//todo check for collision.
	GameServer()->TimeRunController()->SpawnBot(BOTTYPE_HAMMER, m_Pos - vec2(80.0f, 20.0f));
	GameServer()->TimeRunController()->SpawnBot(BOTTYPE_HAMMER, m_Pos + vec2(80.0f, -20.0f));
}

void CBossBot::FreezeAllPlayers() {
	CCharacter* apCloseChars[MAX_CLIENTS];
	int closestChars = GameServer()->m_World.FindEntities(m_Pos, _FMAX, (CEntity**)apCloseChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	for (int i = 0; i < closestChars; i++) {
		CCharacter* character = apCloseChars[i];
		CPlayer* controller = character->GetPlayer();
		if (character->IsAlive() && !controller->IsBot()) {
			character->Freeze(5.0f);
		}
	}
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