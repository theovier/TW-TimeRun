#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include <game/server/botplayer.h>
#include "mine.h"

CMine::CMine(CGameWorld *pGameWorld, vec2 Pos) : CTrap(pGameWorld, Pos) {
	m_RespawnTime = GameServer()->Tuning()->m_MineRespawnTime;
	m_WarnRadius = GameServer()->Tuning()->m_MineWarnRadius;
}

void CMine::Tick() {
	if (Server()->Tick() > m_FireTick && m_Active) {
		Arm();
		WarnNearbyPlayers();
	}
	if (Server()->Tick() > m_RespawnTick && m_RespawnTick > 0 && !m_Active) {
		m_RespawnTick = -1;
		Respawn();
	}
}

void CMine::WarnNearbyPlayers() {
	MakeTickingNoise();
	SetEmotes();
}

void CMine::MakeTickingNoise() {
	CCharacter *pClosest = GameServer()->m_World.ClosestCharacter(m_Pos, m_WarnRadius, NULL);
	if (pClosest) {
		int Mod = (int)(distance(pClosest->GetPos(), m_Pos) / 8);
		if (Mod == 0 || Server()->Tick() % Mod == 0) {
			GameServer()->CreateSound(m_Pos, SOUND_HOOK_ATTACH_GROUND);
		}
	}
}

void CMine::SetEmotes() {
	CCharacter *apCloseChars[MAX_CLIENTS];
	int closestChars = GameServer()->m_World.FindEntities(m_Pos, m_WarnRadius, (CEntity**)apCloseChars, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	for (int i = 0; i < closestChars; i++) {
		CCharacter* possibleVictim = apCloseChars[i];
		SetEmote(possibleVictim);
	}
}

void CMine::SetEmote(CCharacter* passenger) {
	if (!passenger || passenger->GetPlayer()->IsBot() || GameServer()->Collision()->IntersectLine(m_Pos, passenger->GetPos(), NULL, NULL)) {
		return;
	}
	passenger->m_EmoteType = EMOTE_SURPRISE;
	passenger->m_EmoteStop = Server()->Tick() + 1 * Server()->TickSpeed();
	if (Server()->Tick() > passenger->GetPlayer()->m_LastEmote + Server()->TickSpeed() * 2) {
		GameServer()->SendEmoticon(passenger->GetPlayer()->GetCID(), EMOTICON_EXCLAMATION);
		passenger->GetPlayer()->m_LastEmote = Server()->Tick();
	}
}

void CMine::Fire(CCharacter* at) {
	GameServer()->CreateExplosion(m_Pos, -1, WEAPON_WORLD, false);
	GameServer()->CreateSound(m_Pos, SOUND_GRENADE_EXPLODE);
	m_Active = false;
	m_RespawnTick = Server()->Tick() + Server()->TickSpeed() * m_RespawnTime;
}

void CMine::Respawn() {
	m_Active = true;
	GameServer()->CreatePlayerSpawn(m_Pos);
}