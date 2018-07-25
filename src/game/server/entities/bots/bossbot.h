#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CBossBot : public CBotCharacter {

public:
	CBossBot(CGameWorld *pWorld);
	const char *GetDisplayName() override;
protected:
	const int GetBotType() { return BOTTYPE_ENDBOSS; };
};