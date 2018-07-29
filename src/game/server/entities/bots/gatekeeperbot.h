#include <game/server/gamecontext.h>
#include <game/server/entities/bots/botcharacter.h>

class CGatekeeperBot : public CBotCharacter {
	MACRO_ALLOC_POOL_ID()
public:
	CGatekeeperBot(CGameWorld *pWorld);
	void SetDoorIndex(int DoorIndex);
protected:
	void Handle() override;
	void OnDeath(CPlayer* Killer) override;

	const int GetBotType() { return BOTTYPE_GATEKEEPER; };

private:
	int m_DoorIndex;
};