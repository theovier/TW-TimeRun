#include <game/server/entities/bots/botcharacter.h>

class CGunbot : public CBotCharacter {
	MACRO_ALLOC_POOL_ID()
public:
	CGunbot(CGameWorld *pWorld);
private:

	void Move(vec2 Target) override;
	void SelectAppropriateWeapon(float distanceToTarget) override;
	void Fire(vec2 Target) override;
	void Hook(vec2 Target) override {};

	const int GetBotType() { return BOTTYPE_GUN; };
};