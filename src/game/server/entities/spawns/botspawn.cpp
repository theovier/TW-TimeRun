#include <engine/shared/config.h>
#include <game/server/botplayer.h>
#include <game/server/gamecontext.h>
#include "botspawn.h"

CBotSpawn::CBotSpawn(CGameWorld *pGameWorld, vec2 Pos, CGameControllerTimeRun *pController) : CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP) {
	GameWorld()->InsertEntity(this);
	m_Pos = Pos;
	m_RespawnTimer = GameServer()->Tuning()->m_RespawnTimer;
	m_TriggerRadius = 700.0f;
	m_RespawnTick = -1;
	m_Controller = pController;
}

vec2 CBotSpawn::GetPos() {
	return CEntity::GetPos();
}

int CBotSpawn::GetBotType() {
	return m_BotType;
}

void CBotSpawn::OnSpawnlingsDeath() {
	//our spawned character and corresponding player died. prepare to spawn a new one!
	m_AlreadySpawned = false;
	m_RespawnTick = Server()->Tick() + Server()->TickSpeed() * m_RespawnTimer;
}

void CBotSpawn::Tick() {
	if (Server()->Tick() > m_RespawnTick && CanSpawn()) {
		Spawn();
	}
}

void CBotSpawn::Reset() {
	m_RespawnTick = 0;
	m_AlreadySpawned = false;
}

bool CBotSpawn::CanSpawn() {
	bool freeSlot = m_Controller->GetFreePlayerSlotID() != -1;
	bool playerNearby = IsPlayerNearby();
	bool isSpawnFree = !IsSpawnOccupied();
	return freeSlot && playerNearby && isSpawnFree && !m_AlreadySpawned;
}

bool CBotSpawn::IsPlayerNearby() {
	for (int i = 0; i < g_Config.m_SvMaxClients; i++) {
		CPlayer* player = GameServer()->m_apPlayers[i];
		if (player) {
			CCharacter* character = player->GetCharacter();
			if (character && distance(character->GetPos(), m_Pos) <= m_TriggerRadius) {
				return true;
			}
		}
	}
	return false;
}

bool CBotSpawn::IsSpawnOccupied() {
	CEntity *pEnts[1] = {0};
	int overlappingCharacters = GameServer()->m_World.FindEntities(m_Pos, CCharacter::ms_PhysSize, pEnts, 1, CGameWorld::ENTTYPE_CHARACTER);
	return overlappingCharacters > 0;
}

int CBotSpawn::Spawn() {
	m_AlreadySpawned = true;
	return m_Controller->SpawnRespawningBot(this);
}