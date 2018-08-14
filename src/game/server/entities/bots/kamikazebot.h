#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CKamikazebot : public CBotCharacter {

	MACRO_ALLOC_POOL_ID()

public:
	CKamikazebot(CGameWorld *pWorld);
	void GiveNinja() override;

protected:
	void Tick() override;
	virtual void Fire(vec2 Target);
	void Hook(vec2 Target) {};
	void SelectAppropriateWeapon(float distanceToTarget) override;
	void OnDeath(CPlayer* Killer);
	const int GetBotType() { return BOTTYPE_KAMIKAZE; };
	
	float m_TriggerRange;
	float m_ExplosionTick;
	float m_ExplosionDelay;
	virtual void Explode();


};