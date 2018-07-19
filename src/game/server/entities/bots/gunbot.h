#include <game/server/entities/botcharacter.h>

class CGunbot : public CBotCharacter {
public:
	CGunbot(CGameWorld *pWorld);
private:
	void Move(vec2 Target) override;
	void SelectAppropriateWeapon(float distanceToTarget) override;
	void Fire(vec2 Target) override;
};