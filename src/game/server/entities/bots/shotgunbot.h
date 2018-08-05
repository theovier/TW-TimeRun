#include <game/server/entities/bots/botcharacter.h>

class CShotgunBot : public CBotCharacter {
	MACRO_ALLOC_POOL_ID()
public:
	CShotgunBot(CGameWorld *pWorld);
private:

	const float m_ShotgunRange = 480.0f;
	void SelectAppropriateWeapon(float distanceToTarget) override;
	void Fire(vec2 Target) override;
	void Hook(vec2 Target) override {};

	const int GetBotType() { return BOTTYPE_SHOTGUN; };
};