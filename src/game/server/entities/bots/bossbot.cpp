#include "bossbot.h"
#include <game/server/entities/lightning.h>
#include <game/server/entities/loot/movingbosspickup.h>

MACRO_ALLOC_POOL_ID_IMPL(CBossBot, MAX_CLIENTS)

#define RAD 0.017453292519943295769236907684886f

CBossBot::CBossBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_CurrentPhase = PHASE_IDLE;
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_BossLife;
	m_MinionPhaseHPThreshold = GameServer()->Tuning()->m_BossPhaseMinionThreshold;
	m_HealPhaseHPThreshold = GameServer()->Tuning()->m_BossPhaseHealThreshold;
	m_EnragePhaseHPThreshold = GameServer()->Tuning()->m_BossPhaseEnrageThreshold;
	m_Skin = "twinbop";
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

void CBossBot::Tick() {
	CBotCharacter::Tick();
	TickCurrentPhase();
}

void CBossBot::TickCurrentPhase() {
	switch (m_CurrentPhase) {
		case PHASE_IDLE:
			TickIdlePhase();
			break;
		case PHASE_SIMPLE:
			TickSimplePhase();
			break;
		case PHASE_MINIONS:
			TickMinionPhase();
			break;
		case PHASE_HEAL:
			TickHealPhase();
			break;
		case PHASE_ENRAGE:
			TickEnrage();
			break;
	}
}

void CBossBot::TickIdlePhase() {
	if (Server()->Tick() > m_EmoteTick + Server()->TickSpeed() * m_EmoteInterval) {
		SetEmote(EMOTE_BLINK, Server()->Tick() + Server()->TickSpeed() * 2 * m_EmoteInterval);
		SetEmoticon(EMOTICON_ZZZ);
	}
}

void CBossBot::TickSimplePhase() {

}

void CBossBot::TickMinionPhase() {
	//spawn minions every X seconds?
}

void CBossBot::TickHealPhase() {
	//spawn heal every X seconds?
}

void CBossBot::TickEnrage() {
	SetEmote(EMOTE_ANGRY, Server()->Tick() + Server()->TickSpeed());
	SpawnLightning();
}

bool CBossBot::TakeDamage(vec2 Force, int Dmg, int From, int Weapon) {
	bool tookDamage = CCharacter::TakeDamage(Force, Dmg, From, Weapon);
	if (IsAlive()) {
		EnterNextPhase();
	}
	return tookDamage;
}

void CBossBot::OnEnterFight() {
	SetEmoticon(EMOTICON_ZOMG);
	SetEmote(EMOTE_ANGRY, Server()->Tick() + Server()->TickSpeed());
}

void CBossBot::EnterNextPhase() {
	if (m_CurrentPhase == PHASE_IDLE) {
		OnEnterFight();
		EnterSimplePhase();
	}
	else if (m_CurrentPhase == PHASE_SIMPLE && m_Health <= m_MinionPhaseHPThreshold) {
		EnterMinionPhase();
	}
	else if (m_CurrentPhase == PHASE_MINIONS && m_Health <= m_HealPhaseHPThreshold) {
		EnterHealPhase();
	}
	else if (m_CurrentPhase == PHASE_HEAL && m_Health <= m_EnragePhaseHPThreshold) {
		EnterEnragePhase();
	}
}

void CBossBot::EnterSimplePhase() {
	m_CurrentPhase = PHASE_SIMPLE;
	GameServer()->SendBroadcast("Simple Phase", -1);
}

void CBossBot::EnterMinionPhase() {
	m_CurrentPhase = PHASE_MINIONS;
	GameServer()->SendBroadcast("Minion Phase", -1);
	SummonMinions(3);
}

void CBossBot::EnterHealPhase() {
	m_CurrentPhase = PHASE_HEAL;
	GameServer()->SendBroadcast("Heal Phase", -1);
	FreezeAllPlayers();
	HealSelf();
}

void CBossBot::EnterEnragePhase() {
	m_CurrentPhase = PHASE_ENRAGE;
	GameServer()->SendBroadcast("Boss enrages!", -1);
	SetEmoticon(EMOTICON_EXCLAMATION);
	if (m_pPlayer) {
		m_pPlayer->SetRainbow(true);
	}
}

void CBossBot::SummonMinions(int amount) {
	GameServer()->SendBroadcast("Boss calls for help!", -1);
	for (int i = 0; i < amount; i++) {
		vec2 spawnPos = GameWorld()->FindEmptySpot(m_Pos, 150, m_ProximityRadius + 5, 10);
		GameServer()->TimeRunController()->SpawnBot(BOTTYPE_HAMMER, spawnPos);
	}
}

void CBossBot::FreezeAllPlayers() {
	CCharacter* apCloseChars[MAX_CLIENTS];
	int closestChars = GameServer()->m_World.FindEntities(m_Pos, (CEntity**)apCloseChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	for (int i = 0; i < closestChars; i++) {
		CCharacter* character = apCloseChars[i];
		CPlayer* controller = character->GetPlayer();
		if (character->IsAlive() && !controller->IsBot()) {
			character->Freeze(5.0f);
		}
	}
}

void CBossBot::HealSelf() {
	//todo check for collision?
	//todo fancy spawn effect?
	//todo slow down
	
	vec2 startLeft = m_Pos - vec2(100.0f, 150.0f);
	CMovingBossPickup* leftHeart = new CMovingBossPickup(&GameServer()->m_World, startLeft, (CEntity*)this, 3.25f, POWERUP_HEALTH);

	vec2 startMiddle = m_Pos - vec2(0, 150.0f);
	CMovingBossPickup* middleHeart = new CMovingBossPickup(&GameServer()->m_World, startMiddle, (CEntity*)this, 3, POWERUP_HEALTH);

	vec2 startRight = m_Pos + vec2(100.0f, -150.0f);
	CMovingBossPickup* rightHeart = new CMovingBossPickup(&GameServer()->m_World, startRight, (CEntity*)this, 3.25f, POWERUP_HEALTH);
}

void CBossBot::SpawnLightning() {
	for (int i = 0; i < 2; i++) {
		float Angle = frandom() * 360.0f * RAD;
		new CLightning(GameWorld(), m_Pos, vec2(cosf(Angle), sinf(Angle)), 50, 50, GetPlayer()->GetCID(), 1);
	}
}

