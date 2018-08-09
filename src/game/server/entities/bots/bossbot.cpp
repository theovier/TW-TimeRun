#include "bossbot.h"
#include <game/server/entities/lightning.h>
#include <game/server/entities/movingbosspickup.h>

MACRO_ALLOC_POOL_ID_IMPL(CBossBot, MAX_CLIENTS)

#define RAD 0.017453292519943295769236907684886f

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
	SpawnLightning();
}

void CBossBot::SpawnLightning() {
	for (int i = 0; i < 2; i++) {
		float Angle = frandom() * 360.0f * RAD;
		new CLightning(GameWorld(), m_Pos, vec2(cosf(Angle), sinf(Angle)), 50, 50, GetPlayer()->GetCID(), 1);
	}
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
		OnEnterFight();
	}
	return tookDamage;
}

void CBossBot::OnEnterFight() {
	m_InFight = true;
	SetEmoticon(EMOTICON_ZOMG);
	SetEmote(EMOTE_ANGRY, Server()->Tick() + Server()->TickSpeed());
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
	HealSelf();
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

void CBossBot::HealSelf() {
	//todo check if height is enough
	
	vec2 startLeft = m_Pos - vec2(100.0f, 150.0f);
	CMovingBossPickup* leftHeart = new CMovingBossPickup(&GameServer()->m_World, startLeft, m_Pos, 3.25f, POWERUP_HEALTH);

	vec2 startMiddle = m_Pos - vec2(0, 150.0f);
	CMovingBossPickup* middleHeart = new CMovingBossPickup(&GameServer()->m_World, startMiddle, m_Pos, 3, POWERUP_HEALTH);

	vec2 startRight = m_Pos + vec2(100.0f, -150.0f);
	CMovingBossPickup* rightHeart = new CMovingBossPickup(&GameServer()->m_World, startRight, m_Pos, 3.25f, POWERUP_HEALTH);
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