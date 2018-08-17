#include <game/server/entities/bots/botcharacter.h>

class CShotgunBot : public CBotCharacter {
	MACRO_ALLOC_POOL_ID()
public:
	CShotgunBot(CGameWorld *pWorld);

protected:
	

private:

	const float m_ShotgunRange = 480.0f;
	const int m_MaxShotgunAmmo = 2;

	void SelectAppropriateWeapon(float distanceToTarget) override;
	void Handle() override;
	void Aim(vec2 Target) override;
	void StopReload() override;
	const int GetBotType() { return BOTTYPE_SHOTGUN; };

};