#ifndef GAME_SERVER_ENTITY_CHECKPOINT_H
#define GAME_SERVER_ENTITY_CHECKPOINT_H

#include <game/server/entity.h>
#include <game/server/player.h>
#include "character.h"

class CCheckpoint : public CFlag {

public:
	CCheckpoint(CGameWorld *pGameWorld, int Team, vec2 StandPos, int Index);
	virtual void Tick() override;
	virtual void Reset() override;

private:
	int m_Index;
	bool m_Dropped = false;
	float m_RegenerationRadius;
	void SaveProgress(CCharacter* beneficiary, CPlayer* controller);
	void Regenerate(CCharacter* beneficiary, CPlayer* controller);
	bool RegenerateHealth(CCharacter* beneficiary);
	void RegenerateAmmo(CCharacter* beneficiary);
	bool IsGrounded();
	void ReturnToStand();
};

#endif
