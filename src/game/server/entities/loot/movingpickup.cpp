#include <game/server/gamecontext.h>
#include "movingpickup.h"

CMovingPickup::CMovingPickup(CGameWorld *pGameWorld, vec2 From, vec2 To, float Duration, int Type, int SubType) : CPickup(pGameWorld, Type, SubType) {
	m_Pos = From;
	m_Destination = To;
	m_MovementSpeed = 1 / (Server()->TickSpeed() * Duration);
	m_Dir = vec2(To.x - From.x, To.y - From.y);
}

void CMovingPickup::Reset() {
	Despawn();
}

void CMovingPickup::Tick() {
	TickPickup();
	TickMovement();
}

void CMovingPickup::TickMovement() {
	if (!m_DestinationReached) {
		Move();
		m_DestinationReached = HasReachedDestination();
	}
	else {
		m_DestinationReached = true;
	}
}

void CMovingPickup::Move() {
	m_Pos += m_Dir * m_MovementSpeed;
}

void CMovingPickup::Despawn() {
	GameWorld()->DestroyEntity(this);
}

