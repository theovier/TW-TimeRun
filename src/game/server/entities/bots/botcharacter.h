#ifndef GAME_SERVER_BOTCHARACTER_H
#define GAME_SERVER_BOTCHARACTER_H

#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>

class CBotCharacter : public CCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CBotCharacter(CGameWorld *pWorld);
	void Tick() override;
	void Die(int Killer, int Weapon) override;
	virtual const char *GetDisplayName();
	const virtual char* GetSkinName() { return m_Skin; };

protected:
	virtual void Handle();
	virtual vec2 FindTarget();
	virtual void Move(vec2 Target);
	virtual void SelectAppropriateWeapon(float distanceToTarget);
	virtual void Fire(vec2 Target);
	virtual void Aim(vec2 Target);
	virtual void Hook(vec2 Target);
	void StopFire();
	vec2 FindNearestTarget();

	const float m_MeeleRange = 64.0f;
	const float m_HookRange = 512.0f;
	float m_AggroRadius;
	float m_AttackTimer;

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

private:
	void MarkControllerForDestroy();
};

#endif