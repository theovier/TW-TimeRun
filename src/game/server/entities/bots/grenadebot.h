#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CGrenadeBot : public CBotCharacter {
	MACRO_ALLOC_POOL_ID()
public:
	CGrenadeBot(CGameWorld *pWorld);

protected:
	void Hook(vec2 Target) {};
	void SelectAppropriateWeapon(float distanceToTarget) override { m_QueuedWeapon = WEAPON_GRENADE; };
	void Aim(vec2 Target) override;
	void StopReload() override;
	const int GetBotType() { return BOTTYPE_GRENADE; };

private:
	const float m_GrenadeRange = 480.0f;
	const int m_MaxGrenadeAmmo = 10;
};