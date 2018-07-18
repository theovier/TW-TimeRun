#include <game/server/entities/botcharacter.h>

class CGunbot : public CBotCharacter {
public:
	CGunbot(CGameWorld *pWorld);
protected:
	void Move(vec2 Target) override;
	void SelectWeapon(float distanceToTarget) override;
	void Fire(vec2 Target) override;
};