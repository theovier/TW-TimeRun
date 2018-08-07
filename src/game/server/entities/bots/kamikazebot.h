#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CKamikazebot : public CBotCharacter {
public:
	CKamikazebot(CGameWorld *pWorld);
protected:

	void Tick() override;
	void Hook(vec2 Target) {};
	void SelectAppropriateWeapon(float distanceToTarget) override;
	const int GetBotType() { return BOTTYPE_KAMIKAZE; };
};