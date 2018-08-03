#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CHammerbot : public CBotCharacter {
public:
	CHammerbot(CGameWorld *pWorld);
protected:

	void Move(vec2 Target) override;
	void Fire(vec2 Target) override;

	const int GetBotType() { return BOTTYPE_HAMMER; };
};