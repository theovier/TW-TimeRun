#ifndef GAME_SERVER_ENTITIES_MOVINGPICKUP_H
#define GAME_SERVER_ENTITIES_MOVINGPICKUP_H

#include <game/server/entity.h>
#include <game/server/entities/pickup.h>

class CMovingPickup : public CPickup {

public:
	CMovingPickup(CGameWorld *pGameWorld, vec2 From, vec2 To, float Duration, int Type, int SubType = 0);
	virtual void Reset();
	virtual void Tick();

protected:
	vec2 m_Dir;
	vec2 m_Destination;
	float m_MovementSpeed;
	virtual void TickMovement();
	virtual void Move();

private:
	bool m_DestinationReached;
	const bool HasReachedDestination() { return (int)m_Pos.x == (int)m_Destination.x && (int)m_Pos.y == (int)m_Destination.y; };
	void Despawn();
};
#endif
