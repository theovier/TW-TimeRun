#ifndef GAME_SERVER_BOTCHARACTER_H
#define GAME_SERVER_BOTCHARACTER_H

#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>

class CBotCharacter : public CCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CBotCharacter(CGameWorld *pWorld);
	void Tick() override;
	virtual bool TakeDamage(vec2 Force, int Dmg, int From, int Weapon);
	void Die(int Killer, int Weapon) override;
	void Stun(float Seconds);
	virtual const char *GetDisplayName();
	const virtual char* GetSkinName() { return m_Skin; };

protected:
	virtual void Handle();
	virtual class CCharacter* FindTarget();
	virtual void Move(vec2 Target);
	virtual void SelectAppropriateWeapon(float distanceToTarget);
	virtual void Fire(vec2 Target);
	virtual void Aim(vec2 Target);
	virtual void Hook(vec2 Target);
	void StopFire();
	class CCharacter* FindNearestTarget();

	float m_Range;
	const float m_MeeleRange = 64.0f + 36.0f;
	const float m_GunRange = 500.0f;
	const float m_HookRange = 500.0f;
	float m_AggroRadius;
	float m_AttackTimer;
	float m_AttackSpeed = 0.5f;

	const char* m_Skin = "default";
	const virtual int GetBotType() { return BOTTYPE_HAMMER; };
	virtual void OnDeath(CPlayer* Killer);
	virtual void OnOverlapWeaponStrip() { };
	virtual void OnOverlapDoorTrigger() { };

	//bots despawn if no player is around
	virtual bool ShouldDespawn();
	virtual void Despawn();
	float m_DespawnTime = 60.0f;
	float m_DespawnTick;

	float m_EmoteInterval = 1.5f;
	float m_EmoteTick;
	virtual void SetEmoticon(int Emoticon);

	float m_StunTime;
	float m_StunTick;
	float m_StunChance; //1 = always stunned on hit. 0 = never stunned on hit.
	float m_StunDuration;
	void OnStunned();

	float m_ShotgunKnockback;

private:
	void MarkControllerForDestroy();
};

#endif