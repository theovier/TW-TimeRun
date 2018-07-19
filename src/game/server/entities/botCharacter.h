#ifndef GAME_SERVER_BOTCHARACTER_H
#define GAME_SERVER_BOTCHARACTER_H

#include "character.h"

class CBotCharacter : public CCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CBotCharacter(CGameWorld *pWorld);
	void Tick() override;
	void Die(int Killer, int Weapon) override;

protected:
	virtual void Handle();
	virtual vec2 FindTarget();
	virtual void Move(vec2 Target);
	virtual void SelectAppropriateWeapon(float distanceToTarget);
	virtual void Fire(vec2 Target);
	void Aim(vec2 Target);
	void StopFire();
	vec2 FindNearestTarget();

	const float m_MeeleRange = 64.0f;
	float m_AggroRadius;
	float m_AttackTimer;

private:
	void MarkControllerForDestroy();
	void OnDeath(CPlayer* Killer);

};

#endif