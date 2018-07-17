#include <game/server/entities/botcharacter.h>

class CGunbot : public CBotCharacter {
public:
	CGunbot(CGameWorld *pWorld);
protected:
	void Handle() override;
};