#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/gamemodes/timerun.h>
#include "drop.h"

CDrop::CDrop(CGameWorld *pGameWorld, vec2 Pos, int Type, int SubType) : CPickup(pGameWorld, Type, SubType) {	
	CPickup::CPickup(pGameWorld, Type, SubType);
	m_Pos = Pos;
	m_Lifetime = Server()->Tick();
	m_FlashTime = GameServer()->Tuning()->m_DropFlashTime;
	m_SpawnTick = -1;
}

void CDrop::Reset() {
	Despawn();
}

void CDrop::Tick() {
	CPickup::Tick();
	if (ShouldFlash()) {
		Flash();
	}
	if (ShouldDespawn()) {
		Despawn();
	}
}

bool CDrop::ShouldFlash() {
	return Server()->Tick() > m_Lifetime + Server()->TickSpeed() * max(GameServer()->Tuning()->m_PickupLifetime - m_FlashTime, 1.0f);
}

void CDrop::Flash() {
	Server()->Tick() % 10 == 0 ? m_SpawnTick = 999 : m_SpawnTick = -1;
}

bool CDrop::ShouldDespawn() {
	return Server()->Tick() > m_Lifetime + GameServer()->Tuning()->m_PickupLifetime * Server()->TickSpeed();
}

void CDrop::Despawn() {
	GameWorld()->DestroyEntity(this);
}

void CDrop::HandleRespawn(int RespawnTime) {
	GameWorld()->DestroyEntity(this);
}

void CDrop::Snap(int SnappingClient) {
	CPickup::Snap(SnappingClient);
}