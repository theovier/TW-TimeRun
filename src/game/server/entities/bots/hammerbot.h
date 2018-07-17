#include <game/server/entities/botcharacter.h>

class CHammerbot : public CBotCharacter {
public:
	CHammerbot(CGameWorld *pWorld);
protected:
	void Handle() override;
};